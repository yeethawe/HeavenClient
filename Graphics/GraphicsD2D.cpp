/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "Graphics\GraphicsD2D.h"
#include <unordered_map>

namespace Graphics
{
	namespace GraphicsD2D
	{
		IWICImagingFactory** p_factory = nullptr;
		ID2D1BitmapRenderTarget** p_target = nullptr;
		IDWriteFactory** p_dwfactory = nullptr;

		using std::unordered_map;
		unordered_map<Textlabel::Font, IDWriteTextFormat*> fonts;
		unordered_map<Textlabel::Textcolor, ID2D1SolidColorBrush*> brushes;
		unordered_map<size_t, ID2D1Bitmap*> bitmaps;

		void init(IWICImagingFactory** pwf, ID2D1BitmapRenderTarget** pt, IDWriteFactory** dwf)
		{
			p_factory = pwf;
			p_target = pt;
			p_dwfactory = dwf;

			if (!p_dwfactory || !p_target)
				return;

			IDWriteFactory* dwfactory = (*p_dwfactory);
			ID2D1BitmapRenderTarget* target = (*p_target);

			if (!dwfactory || !target)
				return;

			dwfactory->CreateTextFormat(L"Times New Roman", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[Textlabel::DWF_TEXTFIELD]);
			fonts[Textlabel::DWF_TEXTFIELD]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			fonts[Textlabel::DWF_TEXTFIELD]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-us", &fonts[Textlabel::DWF_12ML]);
			fonts[Textlabel::DWF_12ML]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			fonts[Textlabel::DWF_12ML]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-us", &fonts[Textlabel::DWF_12LL]);
			fonts[Textlabel::DWF_12LL]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			fonts[Textlabel::DWF_12LL]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-us", &fonts[Textlabel::DWF_12MC]);
			fonts[Textlabel::DWF_12MC]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			fonts[Textlabel::DWF_12MC]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-us", &fonts[Textlabel::DWF_12BC]);
			fonts[Textlabel::DWF_12BC]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			fonts[Textlabel::DWF_12BC]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-us", &fonts[Textlabel::DWF_12MR]);
			fonts[Textlabel::DWF_12MR]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			fonts[Textlabel::DWF_12MR]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[Textlabel::DWF_14ML]);
			fonts[Textlabel::DWF_14ML]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			fonts[Textlabel::DWF_14ML]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[Textlabel::DWF_14MC]);
			fonts[Textlabel::DWF_14MC]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			fonts[Textlabel::DWF_14MC]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[Textlabel::DWF_14BC]);
			fonts[Textlabel::DWF_14BC]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			fonts[Textlabel::DWF_14BC]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[Textlabel::DWF_14MR]);
			fonts[Textlabel::DWF_14MR]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			fonts[Textlabel::DWF_14MR]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			dwfactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, 20.0f, L"en-us", &fonts[Textlabel::DWF_20MC]);
			fonts[Textlabel::DWF_20MC]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			fonts[Textlabel::DWF_20MC]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brushes[Textlabel::TXC_WHITE]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brushes[Textlabel::TXC_BLACK]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brushes[Textlabel::TXC_YELLOW]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &brushes[Textlabel::TXC_BLUE]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brushes[Textlabel::TXC_RED]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Brown), &brushes[Textlabel::TXC_BROWN]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &brushes[Textlabel::TXC_GREY]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &brushes[Textlabel::TXC_ORANGE]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &brushes[Textlabel::TXC_MBLUE]);
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Violet), &brushes[Textlabel::TXC_VIOLET]);
		}

		void clear()
		{
			for (auto& bmp : bitmaps)
			{
				if (bmp.second)
				{
					bmp.second->Release();
				}
			}

			bitmaps.clear();
		}

		bool available(size_t id)
		{
			return bitmaps.count(id) > 0;
		}

		void addbitmap(const bitmap& bmp)
		{
			if (!p_factory || !p_target || available(bmp.id()))
				return;

			IWICImagingFactory* factory = (*p_factory);
			ID2D1BitmapRenderTarget* target = (*p_target);

			if (!factory || !target)
				return;

			IWICBitmap* wic = nullptr;

			factory->CreateBitmapFromMemory(
				bmp.width(), bmp.height(),
				GUID_WICPixelFormat32bppBGRA,
				4 * bmp.width(),
				bmp.length(),
				(BYTE*)bmp.data(),
				&wic);

			if (!wic)
				return;

			IWICFormatConverter* converter = nullptr;
			IWICBitmap* temp = nullptr;

			int result = factory->CreateFormatConverter(&converter);
			if (result == S_OK)
			{
				converter->Initialize(wic,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone, 0, 0.f,
					WICBitmapPaletteTypeMedianCut);
				factory->CreateBitmapFromSource(converter, WICBitmapNoCache, &temp);
				converter->Release();
			}
			wic->Release();

			target->CreateBitmapFromWicBitmap(temp, &bitmaps[bmp.id()]);
			temp->Release();
			temp = nullptr;
		}

		void draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, float alpha,
			float xscale, float yscale, int16_t ox, int16_t oy) {

			if (!p_target || !bitmaps.count(id))
				return;

			ID2D1BitmapRenderTarget* target = (*p_target);
			ID2D1Bitmap* todraw = bitmaps[id];

			if (!target || !todraw)
				return;

			bool transform = (xscale != 1.0f) || (yscale != 1.0f);
			if (transform)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(xscale, yscale),
					D2D1::Point2F((FLOAT)ox, (FLOAT)oy)
					));
			}

			D2D_RECT_F rect = D2D1::RectF((FLOAT)x, (FLOAT)y, (FLOAT)(x + w), (FLOAT)(y + h));
			target->DrawBitmap(todraw, rect, alpha);

			if (transform)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(1.0f, 1.0f),
					D2D1::Point2F((FLOAT)ox, (FLOAT)oy)
					));
			}
		}

		vector2d<float> createlayout(const wstring& text, Textlabel::Font font, float wmax, map<size_t, float>& advances)
		{
			if (!p_dwfactory)
				return vector2d<float>();

			IDWriteFactory* dwfactory = (*p_dwfactory);
			IDWriteTextFormat* fnt = fonts[font];

			if (!dwfactory || !fnt || text.size() == 0)
				return vector2d<float>();

			if (wmax < 1.0f)
				wmax = 800.0f;

			IDWriteTextLayout* layout = nullptr;
			dwfactory->CreateTextLayout(text.c_str(), (UINT32)text.length(), fnt, wmax, 624, &layout);

			if (!layout)
				return vector2d<float>();

			DWRITE_CLUSTER_METRICS* metrics = new DWRITE_CLUSTER_METRICS[text.length()];
			UINT32 result = 0;

			layout->GetClusterMetrics(metrics, (UINT32)text.size(), &result);
			if (result != text.size())
			{
				layout->GetClusterMetrics(metrics, result, &result);
			}

			float tadv = 0.0f;
			advances.clear();
			for (UINT32 i = 0; i < result; i++)
			{
				advances[i] = tadv;
				tadv += metrics[i].width;
			}
			advances[result] = tadv;

			delete[] metrics;

			DWRITE_TEXT_METRICS tmetrics;
			layout->GetMetrics(&tmetrics);

			float width = (tadv > wmax) ? wmax : tadv;
			float height = tmetrics.height;

			layout->Release();

			return vector2d<float>(width, height);
		}

		void drawtext(const wstring& text, Textlabel::Font font, Textlabel::Textcolor color,
			Textlabel::Background back, float alpha, vector2d<float> pos, vector2d<float> dimensions) {

			if (!p_target)
				return;

			ID2D1BitmapRenderTarget* target = (*p_target);
			IDWriteTextFormat* fnt = fonts[font];
			ID2D1SolidColorBrush* brush = brushes[color];

			if (!target || !fnt || !brush)
				return;

			FLOAT x = pos.x();
			FLOAT y = pos.y() + 5;
			FLOAT width = dimensions.x();
			FLOAT height = dimensions.y();

			if (width == 0.0f)
				width = 800.0f;
			if (height == 0.0f)
				height = fnt->GetFontSize();

			if (x + width < 0.0f || x - width > 800.0f || y + height < 0.0f || y - height > 624.0f || text.size() == 0)
				return;

			D2D1_RECT_F layrect;
			switch (fnt->GetTextAlignment())
			{
			case DWRITE_TEXT_ALIGNMENT_LEADING:
				layrect = D2D1::RectF(x, y, x + width, y + height);
				break;
			case DWRITE_TEXT_ALIGNMENT_TRAILING:
				layrect = D2D1::RectF(x - width, y, x, y + height);
				break;
			case DWRITE_TEXT_ALIGNMENT_CENTER:
				layrect = D2D1::RectF(x - width / 2, y, x + width / 2, y + height);
				break;
			default:
				return;
			}

			if (back == Textlabel::TXB_NAMETAG)
			{
				ID2D1SolidColorBrush* backbrush = brushes[Textlabel::TXC_BLACK];
				if (backbrush)
				{
					FLOAT hwidth = (width / 2) + 2.5f;
					FLOAT space = fnt->GetFontSize() + 2.0f;

					D2D1_RECT_F bglayout = D2D1::RectF(x - hwidth, y + 2, x + hwidth, y + space);
					D2D1_ROUNDED_RECT rect = { bglayout, 1.5f, 1.5f };
					backbrush->SetOpacity(0.6f);
					target->DrawRoundedRectangle(&rect, backbrush);
					target->FillRoundedRectangle(&rect, backbrush);
					backbrush->SetOpacity(1.0f);
				}
			}

			brush->SetOpacity(alpha);
			target->DrawText(text.c_str(), (UINT32)text.size(), fnt, layrect, brush);
			brush->SetOpacity(1.0f);
		}
	}
}