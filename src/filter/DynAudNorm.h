/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: DynAudNorm.h                                                    *
*                                                                        *
*  This file is part of FFQueue.                                         *
*                                                                        *
*  FFQueue is free software: you can redistribute it and/or modify       *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
*  FFQueue is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*  GNU General Public License for more details.                          *
*                                                                        *
*  You should have received a copy of the GNU General Public License     *
*  along with FFQueue.  If not, see <http://www.gnu.org/licenses/>.      *
*                                                                        *
*************************************************************************/

#ifndef DYNAUDNORM_H
#define DYNAUDNORM_H

#include "FilterBasePanel.h"

class DynAudNorm : public FilterBasePanel
{

    public:

        DynAudNorm(wxWindow* parent);
        virtual ~DynAudNorm();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:


        wxSlider *m_FrameLen, *m_GaussWnd, *m_PeakVal, *m_MaxGain,
                 *m_TargetRMS, *m_CompressFactor;

        wxCheckBox *m_ChannelCoupling, *m_DCBiasCorrection, *m_AltBoundaryMode;

        wxStaticText *m_FrameLenText, *m_GaussWndText, *m_PeakValText,
                     *m_MaxGainText, *m_TargetRMSText, *m_CompressFactorText;

        void OnSliderChange(wxCommandEvent &event);

};

#endif // DYNAUDNORM_H
