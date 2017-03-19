/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQFilterEdit.h                                                 *
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

#ifndef FFQFILTEREDIT_H
#define FFQFILTEREDIT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQFilterEdit)
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(FFQFilterEdit)
//*)

#include "filter/FilterBasePanel.h"

class FFQFilterEdit: public wxDialog
{
	public:

		FFQFilterEdit(wxWindow* parent);
		virtual ~FFQFilterEdit();

		bool Execute(LPFFMPEG_FILTER filter);

		//(*Declarations(FFQFilterEdit)
		wxButton* CancelButton;
		wxStaticBoxSizer* FilterContainer;
		wxFlexGridSizer* GridSizer;
		wxButton* OKButton;
		//*)

	protected:

		//(*Identifiers(FFQFilterEdit)
		static const long ID_OKBUTTON;
		static const long ID_CANCELBUTTON;
		//*)

	private:

		//(*Handlers(FFQFilterEdit)
		void OnButtonClick(wxCommandEvent& event);
		//*)

		FilterBasePanel* m_FilterPanel;
		LPFFMPEG_FILTER m_Filter;

		DECLARE_EVENT_TABLE()
};

#endif // FFQFILTEREDIT_H
