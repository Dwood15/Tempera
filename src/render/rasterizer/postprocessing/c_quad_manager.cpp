/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_quad_manager.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		/////////////////////////////////////////////////////////////////////
		// c_quad_instance
		c_quad_instance::s_quad& c_quad_instance::Quad()
		{
			return m_quad;
		}

		uint32 c_quad_instance::GetRef()
		{
			return m_reference_count;
		}

		void c_quad_instance::AddRef()
		{
			m_reference_count++;
		}

		void c_quad_instance::Release()
		{
			m_reference_count--;
		}

		/////////////////////////////////////////////////////////////////////
		// c_quad_manager
		c_quad_manager c_quad_manager::g_quad_manager;

		/*!
		 * \brief
		 * Sets member variables to their default values.
		 * 
		 * \returns
		 * False if a serious error occurred.
		 * 
		 * Sets member variables to their default values.
		 */
		void c_quad_manager::Initialize()
		{
			m_globals.status = Enums::pp_component_status_uninitialised;

			m_globals.m_quad_list = nullptr;

			m_globals.m_buffers.index = nullptr;
			m_globals.m_buffers.vertex = nullptr;
		}
		/*!
		 * \brief
		 * Deletes allocated memory and resets variables to defaults.
		 * 
		 * Deletes allocated memory and resets variables to defaults.
		 */
		void c_quad_manager::Dispose()
		{
			CullUnusedQuads();

			delete m_globals.m_quad_list;
			m_globals.m_quad_list = nullptr;

			m_globals.m_buffers.index = nullptr;
			m_globals.m_buffers.vertex = nullptr;
		}

		/*!
		 * \brief
		 * Removes unused quads and creates the vertex and index buffers.
		 * 
		 * \param parameters
		 * The render devices creation parameters.
		 * 
		 * \returns
		 * False if a serious error occured.
		 * 
		 * Removes unused quads and creates the vertex and index buffers. This function is
		 * called what the game is first loaded, to create the buffers for the built in
		 * post processes.
		 */
		void c_quad_manager::InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters)
		{
			// remove unnecessary quads
			CullUnusedQuads();

			// if the manager system hasn't been forsibly unloaded, create the buffers
			if(!m_globals.m_flags.is_disabled)
				CreateBuffersImpl();
		}
		/*!
		 * \brief
		 * Releases the vertex and index buffers.
		 * 
		 * Releases the vertex and index buffers.
		 */
		void c_quad_manager::OnLostDevice_Base()
		{
			// release the quad buffers
			DestroyBuffersImpl();
		}
		/*!
		 * \brief
		 * Recreates the buffers after a device reset.
		 * 
		 * \param parameters
		 * The new presentation parameters of the render device.
		 * 
		 * \returns
		 * False if a serious error occurred.
		 * 
		 * Recreates the buffers after a device reset.
		 */
		void c_quad_manager::OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters)
		{
			// if the manager system hasn't been forsibly unloaded, create the buffers
			if(!m_globals.m_flags.is_disabled)
				CreateBuffersImpl();
		}
		/*!
		 * \brief
		 * Releases the vertex and index buffers.
		 * 
		 * Releases the vertex and index buffers. This function is called just before the game is closed.
		 */
		void c_quad_manager::ReleaseResources_Base()
		{
			DestroyBuffersImpl();
		}

		/*!
		 * \brief
		 * Recreates the buffers when a map is loaded.
		 * 
		 * \returns
		 * False if a serious error occurs.
		 * 
		 * Recreates the buffers when a map is loaded. This function is called when a new map is loaded,
		 * after any quads required by the post process system have been registered.
		 */
		void c_quad_manager::InitializeResources_Cache()
		{
			// cull any unused quads
			CullUnusedQuads();

			if(!m_globals.m_flags.is_disabled)
				CreateBuffersImpl();
		}
		/*!
		 * \brief
		 * Releases the buffers after a map has been unloaded.
		 * 
		 * Releases the buffers after a map has been unloaded. This function is called when a map is unloaded.
		 * By the time this function is called any cache based post process subsystems should have released the
		 * quad instances they were using, so that they can be culled.
		 */
		void c_quad_manager::ReleaseResources_Cache()
		{
			// release the quad buffers
			DestroyBuffersImpl();
		}

		bool c_quad_manager::IsDisabled()
		{
			return m_globals.m_flags.is_disabled;
		}
		/*!
		 * \brief
		 * The quad managers current status.
		 * 
		 * \returns
		 * A pp_component_status describing the current state of the component.
		 *
		 * The quad managers current status. When pp_component_status_ready the component has been initialised
		 * and can be used. When pp_component_status_uninitialised the buffers are NULL and the manager needs
		 * to be initialised before being used. When pp_component_status_initialisation_failed an error occurred
		 * when initialising and the manager should not be used. When pp_component_status_disabled the used has
		 * forced an unload of the post process system and the manager needs to be initialised before it can be used.
		 */
		Enums::pp_component_status c_quad_manager::ComponentStatus()
		{
			return m_globals.status;
		}

		/*!
		 * \brief
		 * Creates the Direct3D resources for the current quad set.
		 * 
		 * \returns
		 * True if the operation was successful.
		 * 
		 * This function will create the index and vertex buffers for the current set of quads. It should only
		 * be used when reloading the post process system after user has unloaded it, since it does not flush
		 * unused quads before creating the buffers.
		 */
		bool c_quad_manager::CreateBuffers()
		{
			bool success = CreateBuffersImpl();
			m_globals.status = Enums::pp_component_status_uninitialised;
			return success;
		}

		/*!
		 * \brief
		 * Releases the Direct3D resources.
		 * 
		 * Releases the Direct3D resources of the quad manager. This is called when the user forces an unload
		 * of the post process system.
		 */
		void c_quad_manager::DestroyBuffers()
		{
			DestroyBuffersImpl();

			// cull unused quads (should be all quads at this point)
			CullUnusedQuads();

			m_globals.m_flags.is_disabled = true;
		}

		/*!
		 * \brief
		 * Creates the actual D3D resources for the quads.
		 * 
		 * \returns
		 * False if a serious error occurred.
		 * 
		 * Creates the actual D3D resources for the quads.
		 */
		bool c_quad_manager::CreateBuffersImpl()
		{
			bool success = true; // TODO: it'd probably be better code-smell-wise to initialize this to false, then set to true in the loop instead
			do
			{
				IDirect3DDevice9* device = c_post_processing_main::Instance().Globals().render_device;

				// to create a vertex buffer we need the vertex count
				int32 vertex_count = GetVertexCount();
				// to create an index buffer we need the index count
				int32 index_count = GetIndexCount();

				// if there are no quads to be made, return
				if(!vertex_count || !index_count)
					break;

				HRESULT result;
				// create the vertex buffer
				result = device->CreateVertexBuffer(
					vertex_count * sizeof(s_postprocess_vertex),
					D3DUSAGE_WRITEONLY, 
					kVertexFormat, 
					D3DPOOL_DEFAULT, 
					&m_globals.m_buffers.vertex,
					nullptr);

				YELO_ASSERT_DISPLAY(SUCCEEDED(result), "failed to create the post processing vertex buffer");
				if(FAILED(result)) { success = false; break; }

				// create the index buffer
				result = device->CreateIndexBuffer(
					index_count * sizeof(WORD),
					D3DUSAGE_WRITEONLY, 
					D3DFMT_INDEX16, 
					D3DPOOL_DEFAULT, 
					&m_globals.m_buffers.index, 
					nullptr);

				YELO_ASSERT_DISPLAY(SUCCEEDED(result), "failed to create the post processing index buffer");
				if(FAILED(result)) { success = false; break; }

				// lock the vertex buffer
				s_postprocess_vertex* vertices;
				result = m_globals.m_buffers.vertex->Lock( 0, 0, CAST_PTR(void**, &vertices), 0 );

				if(FAILED(result)) { success = false; break; }

				// lock the index buffer
				s_face* indicies;
				result = m_globals.m_buffers.index->Lock( 0, 0, CAST_PTR(void**, &indicies), 0 );

				if(FAILED(result)) { success = false; break; }

				// fill the vertex and index buffers with data, according to the quad list
				int32 vertex_offset = 0;
				int32 index_offset = 0;
				c_quad_instance* quad_instance = m_globals.m_quad_list;
				while(quad_instance)
				{
					AddVertices(vertices,
						quad_instance,
						c_post_processing_main::Instance().Globals().screen_dimensions);
					AddIndices(indicies, quad_instance);

					// the start vertex and index variables indicate where in the data the render device should access
					// to draw the quad
					// this is so that we only need one vertex and index buffer to hold all of the quads
					quad_instance->Quad().start_vertex = vertex_offset;
					quad_instance->Quad().start_index = index_offset;

					vertex_offset += quad_instance->Quad().vertex_count;
					index_offset += quad_instance->Quad().primitive_count * 3;

					quad_instance = quad_instance->GetNext();
				}
				// unlock the buffers
				m_globals.m_buffers.vertex->Unlock();
				m_globals.m_buffers.index->Unlock();
			}
			while(false);
			
			m_globals.m_flags.is_disabled = false;

			return success;
		}

		/*!
		 * \brief
		 * Creates a quad instance to shaders to draw with.
		 * 
		 * \param x_tessellation
		 * How many segments the quad should have on the X axis.
		 * 
		 * \param y_tessellation
		 * How many segments the quad should have on the Y axis.
		 * 
		 * \returns
		 * A pointer to a quad instance.
		 * 
		 * A quad instance details the quad size and vertex/index offsets for the quad in the buffers. The shaders use this
		 * when drawing the quad. If a quad with the matching dimensions does not exist, a new quad instance is created
		 * and returned. Otherwise the pointer of an existing, matching instance is returned. This is to reduce the video
		 * memory footprint as there are no duplicates.
		 */
		c_quad_instance*	c_quad_manager::CreateQuad(const TagGroups::s_effect_postprocess_quad_definition& quad_definition)
		{
			// lock the axis tesselation to the minimum and maximum tessellation if necessary
			c_quad_instance* quad = nullptr;

			point2d tess_final;
			tess_final.x = __max(quad_definition.tessellation.x, 1);
			tess_final.x = __min(tess_final.x, k_maximum_quads_per_axis);
			tess_final.y = __max(quad_definition.tessellation.y, 1);
			tess_final.y = __min(tess_final.y, k_maximum_quads_per_axis);

			real_bounds x_final;
			x_final.lower = __max(quad_definition.x_bounds.lower, 0);
			x_final.lower = __min(x_final.lower, 1);
			x_final.upper = __max(quad_definition.x_bounds.upper, 0);
			x_final.upper = __min(x_final.upper, 1);

			real_bounds y_final;
			y_final.lower = __max(quad_definition.y_bounds.lower, 0);
			y_final.lower = __min(y_final.lower, 1);
			y_final.upper = __max(quad_definition.y_bounds.upper, 0);
			y_final.upper = __min(y_final.upper, 1);

			// look for an existing quad using GetExistingQuad, if found return that quad
			quad = GetExistingQuad(tess_final, quad_definition.x_bounds, quad_definition.y_bounds);

			// if not, add a quad instance to the list and return that
			if(!quad)
			{
				quad = new c_quad_instance(); // TODO: non-trivial type? just use an actual ctor instead of a method
				quad->Ctor(tess_final, quad_definition.x_bounds, quad_definition.y_bounds);

				AppendLinkedListNode(m_globals.m_quad_list, quad);
			}

			// increase the reference count of the quad
			quad->AddRef();
			return quad;
		}

		/*!
		 * \brief
		 * Sets the stream source and index buffer of the device for drawing the quad.
		 * 
		 * \returns
		 * Returns S_OK if successful, otherwise return non-zero.
		 * 
		 * Sets the stream source and index buffer of the device for drawing the quad.
		 */
		HRESULT				c_quad_manager::SetBuffers()
		{
			IDirect3DDevice9* device = c_post_processing_main::Instance().Globals().render_device;

			HRESULT hr = device->SetStreamSource(0, m_globals.m_buffers.vertex, 0, sizeof(s_postprocess_vertex));
			hr |= device->SetIndices(m_globals.m_buffers.index);
			hr |= device->SetFVF(kVertexFormat);
			return hr;
		}

		/*!
		 * \brief
		 * Releases the Direct3D resources.
		 * 
		 * Releases the index and vertex buffers.
		 */
		void c_quad_manager::DestroyBuffersImpl()
		{
			// release the buffers
			Yelo::safe_release(m_globals.m_buffers.vertex);
			Yelo::safe_release(m_globals.m_buffers.index);

			m_globals.status = Enums::pp_component_status_uninitialised;
		}

		/*!
		 * \brief
		 * Removes unused quads from the quad list.
		 * 
		 * Removes unused quads from the quad list.
		 */
		void c_quad_manager::CullUnusedQuads()
		{
			c_quad_instance* quad_instance = m_globals.m_quad_list;
			while(quad_instance)
			{
				c_quad_instance* quad_next = quad_instance->GetNext();

				// if the quad isn't being used
				if(!quad_instance->GetRef())
				{
					RemoveLinkedListNode(m_globals.m_quad_list, quad_instance);

					quad_instance->Dtor();
					delete quad_instance;
				}

				quad_instance = quad_next;
			}
		}

		/*!
		 * \brief
		 * Searches through the quad list for a quad with the matching dimensions.
		 * 
		 * \param x_tessellation
		 * The X axis tesselation to look for.
		 * 
		 * \param y_tessellation
		 * The Y axis tesselation to look for.
		 * 
		 * \returns
		 * A quad instance pointer if a matching quad is found, otherwise NULL.
		 * 
		 * Searches through the quad list for a quad with the matching dimensions
		 */
		c_quad_instance*	c_quad_manager::GetExistingQuad(const point2d tesselation, const real_bounds& x_bounds, const real_bounds& y_bounds)
		{
			c_quad_instance* quad_out = nullptr;
			// loop through quad list
			c_quad_instance* quad_instance = m_globals.m_quad_list;
			while(quad_instance)
			{
				const auto& quad = quad_instance->Quad();
				// compare x and y tesselation
				if(
					quad.tessellation.x == tesselation.x &&
					quad.tessellation.y == tesselation.y &&
					quad.x_bounds.lower == x_bounds.lower &&
					quad.x_bounds.upper == x_bounds.upper &&
					quad.y_bounds.lower == y_bounds.lower &&
					quad.y_bounds.upper == y_bounds.upper)
				{
					// return quad instance pointer if a match is found
					quad_out = quad_instance;
					break;
				}
				quad_instance = quad_instance->GetNext();
			}
			// return NULL if no match is found
			return quad_out;
		}

		/*!
		 * \brief
		 * Gets the number of vertices the vertex buffer needs to hold.
		 * 
		 * \returns
		 * The total vertex count of all the quads.
		 * 
		 * Gets the number of vertices the vertex buffer needs to hold.
		 */
		int32				c_quad_manager::GetVertexCount()
		{
			int32 count = 0;
			// loop through quads adding vertex_count for that quad
			c_quad_instance* quad_instance = m_globals.m_quad_list;
			while(quad_instance)
			{
				count += quad_instance->Quad().vertex_count;
				quad_instance = quad_instance->GetNext();
			}
			return count;
		}

		/*!
		 * \brief
		 * Gets the number of indices the index buffer needs to hold.
		 * 
		 * \returns
		 * The total index count of all the quads.
		 * 
		 * Gets the number of indices the index buffer needs to hold.
		 */
		int32				c_quad_manager::GetIndexCount()
		{
			int32 count = 0;
			// loop through quads adding primitive_count * 3 to get the index count for that quad
			c_quad_instance* quad_instance = m_globals.m_quad_list;
			while(quad_instance)
			{
				count += quad_instance->Quad().primitive_count * 3;
				quad_instance = quad_instance->GetNext();
			}
			return count;
		}

		/*!
		 * \brief
		 * Sets the vertex data for a specific quad instance.
		 * 
		 * \param buffer_pointer
		 * The current point in the vertex data.
		 * 
		 * \param quad
		 * The quad instance to create the vertex values for.
		 * 
		 * \param dimensions
		 * The current screen dimensions.
		 * 
		 * Sets the vertex data for a specific quad instance. The buffer pointer is an instance, so does not need
		 * to be incremented outside of this function.
		 */
		void				c_quad_manager::AddVertices(s_postprocess_vertex*& buffer_pointer,
			c_quad_instance* quad,
			const point2d dimensions)
		{
			// TODO: rename 'quad' to 'quad_instance' and use a const reference to quad_instance.Quad()'s value for better performance

			real x_offset = quad->Quad().x_bounds.lower;
			real x_range = quad->Quad().x_bounds.upper - quad->Quad().x_bounds.lower;
			real x_increment = x_range / quad->Quad().tessellation.x;

			real y_offset = quad->Quad().y_bounds.lower;
			real y_range = quad->Quad().y_bounds.upper - quad->Quad().y_bounds.lower;
			real y_increment = y_range / quad->Quad().tessellation.y;
			
			real x_pos_offset = CAST(real, dimensions.x) * x_offset;
			real x_pos_increment = CAST(real, dimensions.x) * x_increment;

			real y_pos_offset = CAST(real, dimensions.y) * y_offset;
			real y_pos_increment = CAST(real, dimensions.y) * y_increment;
			
			real u_tex_increment = 1.0f / quad->Quad().tessellation.x;
			real v_tex_increment = 1.0f / quad->Quad().tessellation.y;

			// set the vertex data
			for(int y = 0; y < quad->Quad().tessellation.y + 1; y++)
			{
				for(int x = 0; x < quad->Quad().tessellation.x + 1; x++)
				{
					// set the 3D position
					// y is negative because screen position is from the top left (0,0) to bottom right (1,-1)
					// z is an arbitrary number as its irrelevant in an orthographic projection
					buffer_pointer->m_position.x = (x_pos_offset + (x * x_pos_increment)) - 0.5f;
					buffer_pointer->m_position.y = -((y_pos_offset + (y * y_pos_increment)) - 0.5f);
					buffer_pointer->m_position.z = 10.0f;
				
					// quad space texture coordinates
					buffer_pointer->m_texcoord_private[0].i = x * u_tex_increment;
					buffer_pointer->m_texcoord_private[0].j = y * v_tex_increment;

					// screen space texture coordinates
					buffer_pointer->m_texcoord_private[1].i = x_offset + (x * x_increment);
					buffer_pointer->m_texcoord_private[1].j = y_offset + (y * y_increment);

					//quad bounds
					buffer_pointer->m_texcoord_private[2].i = quad->Quad().x_bounds.lower;
					buffer_pointer->m_texcoord_private[2].j = quad->Quad().x_bounds.upper;
					buffer_pointer->m_texcoord_private[2].k = quad->Quad().y_bounds.lower;
					buffer_pointer->m_texcoord_private[2].w = quad->Quad().y_bounds.upper;

					// move on to the next vertex
					buffer_pointer++;
				}
			}
		}

		/*!
		 * \brief
		 * Sets the index data for a specific quad.
		 * 
		 * \param buffer_pointer
		 * The current point in the index data.
		 * 
		 * \param quad
		 * The quad instance to create the index values for.
		 * 
		 * Sets the index data for a specific quad instance. The buffer pointer is an instance, so does not need
		 * to be incremented outside of this function.
		 */
		void				c_quad_manager::AddIndices(s_face*& buffer_pointer, c_quad_instance* quad)
		{
			// creates a two dimensional array containing the indices of each vertex point
			int counter = 0;
			int16** index_array = new int16*[quad->Quad().tessellation.y + 1];
			for(int y = 0; y < quad->Quad().tessellation.y + 1; y++)
			{
				index_array[y] = new int16[quad->Quad().tessellation.x + 1];
				for(int x = 0; x < quad->Quad().tessellation.x + 1; x++)
				{
					index_array[y][x] = counter;
					counter++;
				}
			}

			// loop through tess x and tess y setting the indices according to the index table
			for(int y = 0; y < quad->Quad().tessellation.y; y++)
			{
				for(int x = 0; x < quad->Quad().tessellation.x; x++)
				{
					// the quad faces will always be:
					/*
						face 1

						(x + 0, y + 0)------------(x + 1, y + 0)
							|					 _______/
							|			 _______/
							|	 _______/
							|	/
						(x + 0, y + 1)

						face 2
												  (x + 1, y + 0)
												 _______/	|
										 _______/			|
								 _______/					|
								/							|
						(x + 0, y + 1)------------(x + 1, y + 1)
					*/

					buffer_pointer->m_indices[0] = index_array[ y + 1 ][ x + 0 ];
					buffer_pointer->m_indices[1] = index_array[ y + 0 ][ x + 0 ];
					buffer_pointer->m_indices[2] = index_array[ y + 0 ][ x + 1 ];
					buffer_pointer++;

					buffer_pointer->m_indices[0] = index_array[ y + 0 ][ x + 1 ];
					buffer_pointer->m_indices[1] = index_array[ y + 1 ][ x + 1 ];
					buffer_pointer->m_indices[2] = index_array[ y + 1 ][ x + 0 ];
					buffer_pointer++;
				}
			}

			// delete the index table
			for(int y = 0; y < quad->Quad().tessellation.y + 1; y++)
				delete [] index_array[y];
			delete [] index_array;
		}
	};};
};
#endif