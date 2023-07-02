SUBDIRS=src
FFQ_XDGVER=`xdg-icon-resource --version`
FFQ_EXENAME=ffqueue
FFQ_RES=../res

.ONESHELL:
install-data-hook:
	 @FFQ_DST=/usr/share
	 @if test "$(USER)" != "root"; then FFQ_DST=`realpath ~/`/.local/share; fi
	 @if test -z "$(FFQ_XDGVER)"; then
	   @FFQ_IR=$$FFQ_DST/icons/hicolor
	   @FFQ_ID=$$FFQ_IR/16x16/apps
	   @mkdir -p $$FFQ_ID
	   @cp $(FFQ_RES)/MainLogo16.png $$FFQ_ID/$(FFQ_EXENAME).png
	   @FFQ_ID=$$FFQ_IR/32x32/apps
	   @mkdir -p $$FFQ_ID
	   @cp $(FFQ_RES)/MainLogo32.png $$FFQ_ID/$(FFQ_EXENAME).png
	   @FFQ_ID=$$FFQ_IR/64x64/apps
	   @mkdir -p $$FFQ_ID
	   @cp $(FFQ_RES)/MainLogo64.png $$FFQ_ID/$(FFQ_EXENAME).png
	   @mkdir -p $$FFQ_DST/applications
	   @cp $(FFQ_RES)/$(FFQ_EXENAME).desktop $$FFQ_DST/applications/
	 @else
	   @xdg-icon-resource install --novendor --size 16 $(FFQ_RES)/MainLogo16.png $(FFQ_EXENAME)
	   @xdg-icon-resource install --novendor --size 32 $(FFQ_RES)/MainLogo32.png $(FFQ_EXENAME)
	   @xdg-icon-resource install --novendor --size 64 $(FFQ_RES)/MainLogo64.png $(FFQ_EXENAME)
	   @xdg-desktop-menu install --novendor $(FFQ_RES)/$(FFQ_EXENAME).desktop
	 @fi
	 @FFQ_DF=/usr/share/applications/$(FFQ_EXENAME).desktop
	 @if test -e $$FFQ_DF; then chmod a+r $$FFQ_DF; fi
	 @FFQ_DF=/usr/share/applications/$(FFQ_EXENAME).desktop
	 @if test -e $$FFQ_DF; then chmod a+r $$FFQ_DF; fi

.ONESHELL:
uninstall-hook:
	 @FFQ_DST=/usr/share
	 @if test "$(USER)" != "root"; then FFQ_DST=`realpath ~/`/.local/share; fi
	 @if test -z "$(FFQ_XDGVER)" ; then
	   @FFQ_IR=$$FFQ_DST/icons/hicolor
	   @FFQ_ID=$$FFQ_IR/16x16/apps
	   @rm -f $$FFQ_ID/$(FFQ_EXENAME).png
	   @FFQ_ID=$$FFQ_IR/32x32/apps
	   @rm -f $$FFQ_ID/$(FFQ_EXENAME).png
	   @FFQ_ID=$$FFQ_IR/64x64/apps
	   @rm -f $$FFQ_ID/$(FFQ_EXENAME).png
	   @rm -f $$FFQ_DST/applications/$(FFQ_EXENAME).desktop
	 @else
	   @xdg-icon-resource uninstall --novendor --size 16 $(FFQ_EXENAME)
	   @xdg-icon-resource uninstall --novendor --size 32 $(FFQ_EXENAME)
	   @xdg-icon-resource uninstall --novendor --size 64 $(FFQ_EXENAME)
	   @xdg-desktop-menu uninstall --novendor $(FFQ_EXENAME).desktop
	 @fi
