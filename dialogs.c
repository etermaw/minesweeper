#include <windowsx.h>
#include <stdbool.h>

#include "dialogs.h"
#include "resource.h"
#include "config.h"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

//all statics and dialog functions are private!
static game_params* params;
static size_t* wpp;
static size_t* lpp;
static bool new_msg;

BOOL CALLBACK options_func(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK stats_func(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

bool menu_func(ALLEGRO_DISPLAY* wnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *result, void *userdata)
{
	auto instance = GetModuleHandle(NULL);
	auto parent = al_get_win_window_handle(wnd);
	
	if (msg == WM_COMMAND)
	{
		switch (LOWORD(wp))
		{
			case ID_MENU_OPTIONS:
				DialogBox(instance, MAKEINTRESOURCE(IDD_DIALOG1), parent, options_func);
				break;

			case ID_MENU_STATS:
				DialogBox(instance, MAKEINTRESOURCE(IDD_DIALOG2), parent, stats_func);
				break;

			case ID_MENU_NEWGAME:
				new_msg = true;
				break;
		}
	}

	*result = true;

	return false;
}

BOOL CALLBACK options_func(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			HWND combo = GetDlgItem(hwnd, IDC_COMBO1);
			int size = ARRAY_SIZE(field_lookup);

			for (int i = 0; i < size; ++i)
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)field_lookup[i].text);

			if (params->field_size != F_USER)
				ComboBox_SetCurSel(combo, params->field_size);

			else
			{
				ComboBox_Enable(combo, FALSE);
				SetDlgItemInt(hwnd, IDC_EDIT1, params->width, FALSE);
				SetDlgItemInt(hwnd, IDC_EDIT3, params->height, FALSE);
			}

			combo = GetDlgItem(hwnd, IDC_COMBO2);
			size = ARRAY_SIZE(diff_lookup);

			for (int i = 0; i < size; ++i)
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)diff_lookup[i].text);

			CheckDlgButton(hwnd, params->field_size != F_USER ? IDC_RADIO1 : IDC_RADIO2, BST_CHECKED);
			CheckDlgButton(hwnd, params->difficulty_lvl != D_USER ? IDC_RADIO3 : IDC_RADIO4, BST_CHECKED);

			if (params->difficulty_lvl != D_USER)
				ComboBox_SetCurSel(combo, params->difficulty_lvl);

			else
			{
				ComboBox_Enable(combo, FALSE);
				SetDlgItemInt(hwnd, IDC_EDIT2, params->mines, FALSE);
			}

			return FALSE;

			break;
		}

		case WM_COMMAND:
		{
			if (HIWORD(wp) == BN_CLICKED)
			{
				switch (LOWORD(wp))
				{
					case IDC_RADIO1:
						ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO1), TRUE);
						break;
					
					case IDC_RADIO2:
						ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO1), FALSE);
						break;

					case IDC_RADIO3:
						ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO2), TRUE);
						break;

					case IDC_RADIO4:
						ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO2), FALSE);
						break;					
				}
			}

			else if (HIWORD(wp) == EN_SETFOCUS)
			{
				switch (LOWORD(wp))
				{
					case IDC_EDIT1:
					case IDC_EDIT3:
						CheckDlgButton(hwnd, IDC_RADIO1, BST_UNCHECKED);
						CheckDlgButton(hwnd, IDC_RADIO2, BST_CHECKED);
						SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDC_RADIO2, BN_CLICKED), 0);
						break;

					case IDC_EDIT2:
						CheckDlgButton(hwnd, IDC_RADIO3, BST_UNCHECKED);
						CheckDlgButton(hwnd, IDC_RADIO4, BST_CHECKED);
						SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDC_RADIO4, BN_CLICKED), 0);
						break;
				}
			}

			switch (LOWORD(wp))
			{
				case IDOK:
				{
					game_params new_params;
					new_params.width = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, FALSE);
					new_params.height = GetDlgItemInt(hwnd, IDC_EDIT3, NULL, FALSE);
					new_params.mines = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, FALSE);

					if (Button_GetState(GetDlgItem(hwnd, IDC_RADIO1)) == BST_CHECKED)
						new_params.field_size = (FIELD_SIZE)ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO1));

					else
						new_params.field_size = F_USER;

					if (Button_GetState(GetDlgItem(hwnd, IDC_RADIO3)) == BST_CHECKED)
						new_params.difficulty_lvl = (DIFF_LVL)ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO2));

					else
						new_params.difficulty_lvl = D_USER;

					if (memcmp(params, &new_params, sizeof(game_params)) != 0)
					{						
						memcpy(params, &new_params, sizeof(game_params));
						new_msg = true;
					}

					EndDialog(hwnd, 1);
					break;
				}

				case IDCANCEL:
					EndDialog(hwnd, 0);
					break;
			}

			break;
		}
		
		default:
			return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK stats_func(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			HWND combo = GetDlgItem(hwnd, IDC_COMBO1);
			int size = ARRAY_SIZE(diff_lookup);

			for (int i = 0; i < size; ++i)
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)diff_lookup[i].text);

			int pos = params->difficulty_lvl != D_USER ? params->difficulty_lvl : 0;

			ComboBox_SetCurSel(combo, pos);
			
			SetDlgItemInt(hwnd, IDC_EDIT4, wpp[pos] + lpp[pos], FALSE);
			SetDlgItemInt(hwnd, IDC_EDIT5, wpp[pos], FALSE);
			SetDlgItemInt(hwnd, IDC_EDIT6, lpp[pos], FALSE);
			SetDlgItemInt(hwnd, IDC_EDIT7, (wpp[pos] / (float)(wpp[pos] + lpp[pos])) * 100, FALSE);

			break;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wp) == IDOK || LOWORD(wp) == IDCANCEL)
				EndDialog(hwnd, 0);

			if (HIWORD(wp) == CBN_SELCHANGE)
			{
				int pos = ComboBox_GetCurSel((HWND)lp);

				SetDlgItemInt(hwnd, IDC_EDIT4, wpp[pos]+lpp[pos], FALSE);
				SetDlgItemInt(hwnd, IDC_EDIT5, wpp[pos], FALSE);
				SetDlgItemInt(hwnd, IDC_EDIT6, lpp[pos], FALSE);
				SetDlgItemInt(hwnd, IDC_EDIT7, (wpp[pos] / (float)(wpp[pos]+lpp[pos])) * 100, FALSE);
			}
		}

		default:
			return FALSE;
	}

	return TRUE;
}

void set_dialog_params(game_params* params_ptr, size_t* wins_ptr, size_t* l_ptr)
{
	params = params_ptr;
	wpp = wins_ptr;
	lpp = l_ptr;
}

bool is_new_dialog_msg()
{
	bool ret = new_msg;
	new_msg = false;

	return ret;
}

#undef ARRAY_SIZE()
