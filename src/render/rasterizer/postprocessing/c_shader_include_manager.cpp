/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_shader_include_manager.hpp"

#if !PLATFORM_IS_DEDI
#include "Common/FileIO.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		/*!
		 * \brief
		 * Loads a file included in a shader into memory.
		 * 
		 * \param IncludeType
		 * The type of file being included.
		 * 
		 * \param pFileName
		 * The relative path of the file being included
		 * 
		 * \param pParentData
		 * The shader file that includes the requested file.
		 * 
		 * \param ppData
		 * A pointer to a char* that the data needs to be read to.
		 * 
		 * \param pBytes
		 * The size of the included file.
		 * 
		 * \returns
		 * Returns S_OK if the file exists and has been loaded, otherwise E_FAIL.
		 * 
		 * The Open function creates an absolute path to the included file, then 
		 * loads it into memory for the compiler to use.
		 */
		HRESULT API_FUNC c_shader_include_manager::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT*pBytes)
		{
			// create the file location path
			char location[MAX_PATH];
			location[0] = 0;
			strcat_s(location, MAX_PATH, m_include_path);
			strcat_s(location, MAX_PATH, pFileName);

			FileIO::s_file_info info;
			// open the file for reading
			if(FileIO::OpenFile(info, location) != Enums::_file_io_open_error_none)
				return E_FAIL;

			HRESULT success = E_FAIL;
			// read the file
			do
			{
				if(FileIO::ReadFileToMemory(info, CAST_QUAL(LPVOID*, ppData)) != Enums::_file_io_read_error_none)
					break;
				*pBytes = info.file_size;

				success = S_OK;
			}while(false);

			FileIO::CloseFile(info);

			return success;
		}

		/*!
		 * \brief
		 * Deletes the memory allocated for the include file in Open.
		 * 
		 * \param pData
		 * The pointer that contains the file data.
		 * 
		 * \returns
		 * Always returns S_OK.
		 * 
		 * Deletes the memory allocated for the include file in Open.
		 */
		HRESULT API_FUNC c_shader_include_manager::Close(LPCVOID pData)
		{
			delete [] pData;
			return S_OK;
		}
	};};
};
#endif