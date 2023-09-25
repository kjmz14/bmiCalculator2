/*
 * MIT License
 *
 * Copyright (c) 2023 Karol Zimmer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "dlgproc.h"
#include "../res/res.h"
#include "strings.h"
#include "aboutproc.h"
#include "bmi.h"
#include <wchar.h>

#define BG_COLOR (COLORREF)0x00457584

extern HINSTANCE hInst;

static void updateMenu(HMENU hMenu)
{

	int toCheck = IDM_METRIC,
		toUncheck = IDM_IMPERIAL;

	if (BMIUnits == BMIImperial)
	{
		toCheck = IDM_IMPERIAL;
		toUncheck = IDM_METRIC;
	}

	CheckMenuItem(hMenu, toCheck, MF_CHECKED);
	CheckMenuItem(hMenu, toUncheck, MF_UNCHECKED);

}

static void clearTextBoxes(HWND hWnd)
{

	SetDlgItemTextW(hWnd, IDC_HEIGHT1, NULL);
	SetDlgItemTextW(hWnd, IDC_HEIGHT2, NULL);
	SetDlgItemTextW(hWnd, IDC_MASS, NULL);

}

static void updateControls(HWND hWnd)
{

	HWND hHeight2Box = GetDlgItem(hWnd, IDC_HEIGHT2);
	HWND hHeight2Label = GetDlgItem(hWnd, IDC_HEIGHT2_LABEL);

	clearTextBoxes(hWnd);

	if (BMIUnits == BMIMetric)
	{
		SetDlgItemTextW(hWnd, IDC_HEIGHT1_LABEL, strCm);
		SetDlgItemTextW(hWnd, IDC_MASS_LABEL, strKg);
		SendDlgItemMessageW(hWnd, IDC_HEIGHT1, EM_SETLIMITTEXT, 3, 0);
		ShowWindow(hHeight2Box, SW_HIDE);
		ShowWindow(hHeight2Label, SW_HIDE);
		return;
	}
	/* else */
	SetDlgItemTextW(hWnd, IDC_HEIGHT1_LABEL, strFt);
	SetDlgItemTextW(hWnd, IDC_MASS_LABEL, strLbs);
	SendDlgItemMessageW(hWnd, IDC_HEIGHT1, EM_SETLIMITTEXT, 1, 0);
	ShowWindow(hHeight2Box, SW_SHOW);
	ShowWindow(hHeight2Label, SW_SHOW);

}

INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	lParam;

	static HMENU hMenu;
	static HICON hIcon;
	static HBRUSH hBrush;

	switch (uMsg)
	{

	case WM_INITDIALOG:
		/* Background */
		hBrush = CreateSolidBrush(BG_COLOR);
		/* Icon */
		hIcon = LoadIconW(hInst, MAKEINTRESOURCEW(IDI_ICON_GROUP));
		SendMessageW(hWnd, WM_SETICON, 1, (LPARAM)hIcon);
		/* Menu */
		hMenu = LoadMenuW(hInst, MAKEINTRESOURCEW(IDM_BMI));
		SetMenu(hWnd, hMenu);
		/* Window height */
		{
			int menuHeight = GetSystemMetrics(SM_CYMENU);

			RECT rect;
			GetWindowRect(hWnd, &rect);

			int newWindowHeight = menuHeight + rect.bottom - rect.top;

			SetWindowPos(hWnd,
				NULL,
				rect.left,
				rect.top,
				rect.right - rect.left,
				newWindowHeight,
				SWP_NOMOVE | SWP_NOZORDER);
		}
		/* Text boxes limit (universal for both unit systems) */
		SendDlgItemMessageW(hWnd, IDC_HEIGHT2, EM_SETLIMITTEXT, 2, 0);
		SendDlgItemMessageW(hWnd, IDC_MASS, EM_SETLIMITTEXT, 3, 0);

		updateMenu(hMenu);
		updateControls(hWnd);
		break;

	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
	{
		HDC hDC = (HDC)wParam;
		SetTextColor(hDC, 0x00FFFFFF);
		SetBkColor(hDC, BG_COLOR);
	}
		return (INT_PTR)hBrush;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);
		FillRect(hDC, &ps.rcPaint, hBrush);
		EndPaint(hWnd, &ps);
	}
		break;

	case WM_ERASEBKGND:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

			/* Calculating BMI */
		case IDOK:
		{
			BMIHeightStruct heightStruct;
			WCHAR height1[4], height2[3], massBox[4], bmiValueStr[5];
			unsigned int mass;
			double bmiValue;

			ZeroMemory(&heightStruct, sizeof(heightStruct));
			ZeroMemory(height1, sizeof(height1));
			ZeroMemory(height2, sizeof(height2));
			ZeroMemory(massBox, sizeof(massBox));
			ZeroMemory(bmiValueStr, sizeof(bmiValueStr));

			GetDlgItemTextW(hWnd, IDC_HEIGHT1, height1, 4);
			GetDlgItemTextW(hWnd, IDC_HEIGHT2, height2, 3);
			GetDlgItemTextW(hWnd, IDC_MASS, massBox, 4);

			heightStruct.cmFt = (unsigned int)wcstol(height1, 0, 10);
			heightStruct.in = (unsigned int)wcstol(height2, 0, 10);
			mass = (unsigned int)wcstol(massBox, 0, 10);

			if ((heightStruct.cmFt == 0 && heightStruct.in == 0) || mass == 0)
			{
				MessageBoxW(hWnd, strIncorrect, strValue, MB_ICONERROR);
				clearTextBoxes(hWnd);
				break;
			}

			bmiValue = BMICalculate(heightStruct, mass);

			swprintf(bmiValueStr, 5, L"%.1f", bmiValue);
			MessageBoxW(hWnd, bmiValueStr, strValue, MB_OK);

			clearTextBoxes(hWnd);
		}
			return TRUE;

		case IDM_METRIC:
			BMIUnits = BMIMetric;
			break;

		case IDM_IMPERIAL:
			BMIUnits = BMIImperial;
			break;

		case IDM_ABOUT:
			DialogBoxW(hInst, MAKEINTRESOURCEW(IDD_ABOUT), hWnd, AboutProc);
			return TRUE;

		default:
			return TRUE;

		}

		updateMenu(hMenu);
		updateControls(hWnd);
		break;

	case WM_CLOSE:
		EndDialog(hWnd, IDOK);
		break;

	case WM_DESTROY:
		DestroyIcon(hIcon);
		DestroyMenu(hMenu);
		DeleteObject(hBrush);
		break;

	default:
		return FALSE;

	}

	return TRUE;

}
