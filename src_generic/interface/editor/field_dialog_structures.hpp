/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

class CWnd;
class CDataExchange;
class CString;

namespace Yelo
{
	namespace Guerilla
	{
		// IDD_* - Dialog ID
		// IDC_* - Control ID

		// NOTE: Bungie defines their own controls for the roles of CEdit,
		// which derive from CWnd. So you can't just treat the edit control of a field as as CEdit
		enum {
			// string_dialog's value (field_dialog_named) IDC
			k_idc_tag_string_value = 0x65,
		};

		enum /*field_dialog*/ {
			k_idc_field_dialog_name = 0x154,	// field_dialog_named's 'name' IDC
			k_idc_field_dialog_units = 0x17F,	// field_dialog_named_with_units's 'units' IDC

			k_field_dialog_field_information_offset = 0x70,	// field_information*
		};

		enum /*tag_reference_dialog*/ { // sizeof(0x510)
			k_idc_tag_reference_name = 0x140,	// tag_reference_dialog's tag-name IDC
			k_idc_tag_reference_group = 0x141,	// tag_reference_dialog's group-tag IDC

			k_tag_reference_dialog_group_control_offset = 0x3D0,	// FieldCComboBox
			k_tag_reference_dialog_name_control_offset = 0x420,		// FieldCEdit
			k_tag_reference_dialog_browse_button_offset = 0x470,	// FieldCButton
			k_tag_reference_dialog_open_button_offset = 0x4C0,		// FieldCButton
		};
	};
};