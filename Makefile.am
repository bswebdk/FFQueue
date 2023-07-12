SUBDIRS=src
FFQ_BINARY=ffqueue
FFQ_XDGVER=`xdg-icon-resource --version`
if FFQ_BASE_DIR
FFQ_RES=res
else
FFQ_RES=../res
endif
FFQ_DEST=$(DESTDIR)/usr/share
FFQ_ICONDEST=$(FFQ_DEST)/icons/hicolor

.ONESHELL:
install-data-hook:
	 @if [ -z "$(FFQ_XDGVER)" ]; then
	   @FFQ_ID=$(FFQ_ICONDEST)/16x16/apps
	   @mkdir -p $$FFQ_ID
	   @cp $(FFQ_RES)/MainLogo16.png $$FFQ_ID/$(FFQ_BINARY).png
	   @FFQ_ID=$(FFQ_ICONDEST)/32x32/apps
	   @mkdir -p $$FFQ_ID
	   @cp $(FFQ_RES)/MainLogo32.png $$FFQ_ID/$(FFQ_BINARY).png
	   @FFQ_ID=$(FFQ_ICONDEST)/64x64/apps
	   @mkdir -p $$FFQ_ID
	   @cp $(FFQ_RES)/MainLogo64.png $$FFQ_ID/$(FFQ_BINARY).png
	   @mkdir -p $(FFQ_DEST)/applications
	   @cp $(FFQ_RES)/$(FFQ_BINARY).desktop $(FFQ_DEST)/applications/
	 @else
	   @xdg-icon-resource install --novendor --size 16 $(FFQ_RES)/MainLogo16.png $(FFQ_BINARY)
	   @xdg-icon-resource install --novendor --size 32 $(FFQ_RES)/MainLogo32.png $(FFQ_BINARY)
	   @xdg-icon-resource install --novendor --size 64 $(FFQ_RES)/MainLogo64.png $(FFQ_BINARY)
	   @xdg-desktop-menu install --novendor $(FFQ_RES)/$(FFQ_BINARY).desktop
	 @fi
	 @FFQ_DF=$(FFQ_DEST)/applications/$(FFQ_BINARY).desktop
	 @if [ -f "$$FFQ_DF" ]; then chmod a+r "$$FFQ_DF"; fi

.ONESHELL:
uninstall-hook:
	 @if [ -z "$(FFQ_XDGVER)" ]; then
	   @rm -f $(FFQ_ICONDEST)/16x16/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_ICONDEST)/32x32/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_ICONDEST)/64x64/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_DEST)/applications/$(FFQ_BINARY).desktop
	 @else
	   @xdg-icon-resource uninstall --novendor --size 16 $(FFQ_BINARY)
	   @xdg-icon-resource uninstall --novendor --size 32 $(FFQ_BINARY)
	   @xdg-icon-resource uninstall --novendor --size 64 $(FFQ_BINARY)
	   @xdg-desktop-menu uninstall --novendor $(FFQ_BINARY).desktop
	 @fi
