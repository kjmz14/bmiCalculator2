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

	CheckMenuItem(hMenu, IDM_METRIC, MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_IMPERIAL_US, MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_IMPERIAL_UK, MF_UNCHECKED);

	switch (BMIUnits)
	{

	case BMIMetric:
		CheckMenuItem(hMenu, IDM_METRIC, MF_CHECKED);
		break;

	case BMIImperialUS:
		CheckMenuItem(hMenu, IDM_IMPERIAL_US, MF_CHECKED);
		break;

	case BMIImperialUK:
		CheckMenuItem(hMenu, IDM_IMPERIAL_UK, MF_CHECKED);
		break;

	}

}

static void clearTextBoxes(HWND hWnd)
{

	SetDlgItemTextW(hWnd, IDC_HEIGHT1, NULL);
	SetDlgItemTextW(hWnd, IDC_HEIGHT2, NULL);
	SetDlgItemTextW(hWnd, IDC_MASS1, NULL);
	SetDlgItemTextW(hWnd, IDC_MASS2, NULL);

}

static void updateControls(HWND hWnd)
{

	HWND hHeight2Box = GetDlgItem(hWnd, IDC_HEIGHT2),
		hHeight2Label = GetDlgItem(hWnd, IDC_HEIGHT2_LABEL),
		hMass2Box = GetDlgItem(hWnd, IDC_MASS2),
		hMass2Label = GetDlgItem(hWnd, IDC_MASS2_LABEL);

	clearTextBoxes(hWnd);

	/* Only Imperial (UK) uses these */
	if (BMIUnits != BMIImperialUK)
	{
		ShowWindow(hMass2Box, SW_HIDE);
		ShowWindow(hMass2Label, SW_HIDE);
	}
	else
	{
		SetDlgItemTextW(hWnd, IDC_MASS1_LABEL, strSt);
		ShowWindow(hMass2Box, SW_SHOW);
		ShowWindow(hMass2Label, SW_SHOW);
		goto skipSwitch;
	}

	switch (BMIUnits)
	{

	case BMIMetric:
		SetDlgItemTextW(hWnd, IDC_HEIGHT1_LABEL, strCm);
		SetDlgItemTextW(hWnd, IDC_MASS1_LABEL, strKg);
		SendDlgItemMessageW(hWnd, IDC_HEIGHT1, EM_SETLIMITTEXT, 3, 0);
		ShowWindow(hHeight2Box, SW_HIDE);
		ShowWindow(hHeight2Label, SW_HIDE);
		return;

	case BMIImperialUS:
		SetDlgItemTextW(hWnd, IDC_MASS1_LABEL, strLbs);
		break;

	default:
		break;

	}

skipSwitch:
	/* Common for Imperial (US) and Imperial (UK) */
	SetDlgItemTextW(hWnd, IDC_HEIGHT1_LABEL, strFt);
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
		/* Strings */
		BC2StringsLoad();
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
		/* Text boxes limits (universal for both unit systems) */
		SendDlgItemMessageW(hWnd, IDC_HEIGHT2, EM_SETLIMITTEXT, 2, 0);
		SendDlgItemMessageW(hWnd, IDC_MASS1, EM_SETLIMITTEXT, 3, 0);
		SendDlgItemMessageW(hWnd, IDC_MASS2, EM_SETLIMITTEXT, 2, 0);

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
			BMIMassStruct massStruct;
			WCHAR height1[4], height2[3], mass1[4], mass2[2], bmiValueStr[5];
			double bmiValue;

			GetDlgItemTextW(hWnd, IDC_HEIGHT1, height1, 4);
			GetDlgItemTextW(hWnd, IDC_HEIGHT2, height2, 3);
			GetDlgItemTextW(hWnd, IDC_MASS1, mass1, 4);
			GetDlgItemTextW(hWnd, IDC_MASS2, mass2, 4);

			heightStruct.cmFt = wcstol(height1, 0, 10);
			heightStruct.in = wcstol(height2, 0, 10);
			massStruct.kgLbsSt = wcstol(mass1, 0, 10);
			massStruct.lbs = wcstol(mass2, 0, 10);

			if ((heightStruct.cmFt + heightStruct.in) == 0 ||
				(massStruct.kgLbsSt + massStruct.lbs) == 0)
			{
				MessageBoxW(hWnd, strIncorrect, strValue, MB_ICONWARNING);
				clearTextBoxes(hWnd);
				break;
			}

			bmiValue = BMICalculate(heightStruct, massStruct);

			swprintf(bmiValueStr, 5, L"%.1f", bmiValue);
			MessageBoxW(hWnd, bmiValueStr, strValue, MB_OK);

			clearTextBoxes(hWnd);

		}
			return TRUE;

		case IDM_METRIC:
			BMIUnits = BMIMetric;
			break;

		case IDM_IMPERIAL_US:
			BMIUnits = BMIImperialUS;
			break;

		case IDM_IMPERIAL_UK:
			BMIUnits = BMIImperialUK;
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
