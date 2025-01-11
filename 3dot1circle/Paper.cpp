#include "Paper.h"
#include "Common.h"

Paper::Paper(CRect rect)
{
	m_CImage = new CImage();
	m_CImage->Create(WIDTH, HEIGHT, BPP8);
	if (m_CImage->GetBPP() == BPP8)
	{
		static RGBQUAD rgb[256];
		for (unsigned int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_CImage->SetColorTable(0, 256, rgb);
	}
}

Paper::~Paper()
{
	if (m_CImage != 0) {
		delete m_CImage;
	}
}

void Paper::Draw(CClientDC* dc)
{
	m_CImage->Draw(*dc, 0, PADDING);
}

void Paper::Erase()
{
	int nWidth = m_CImage->GetWidth();
	int nHeight = m_CImage->GetHeight();
	int nPitch = m_CImage->GetPitch();

	unsigned char* fm = static_cast<unsigned char*>(m_CImage->GetBits());
	for (int i = 0; i < nHeight; i++) {
		for (int j = 0; j < nWidth; j++) {
			fm[i * nPitch + j] = WHITE;
		}
	}
}