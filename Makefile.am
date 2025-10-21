SUBDIRS:=src
FFQ_BINARY:=ffqueue
FFQ_XDGVER:=`xdg-icon-resource --version 2> /dev/null`
if FFQ_BASE_DIR
FFQ_RES:=res
else
FFQ_RES:=../res
endif
if DESTDIR
FFQ_DEST:=$(DESTDIR)/share
else
FFQ_DEST:=$(prefix)/share
endif
FFQ_ICONDEST:=$(FFQ_DEST)/icons/hicolor

.ONESHELL:
install-data-hook:
	 @if [ -z "$(FFQ_XDGVER)" ]; then
	   @install -Dm644 $(FFQ_RES)/MainLogo16.png $(FFQ_ICONDEST)/16x16/apps/$(FFQ_BINARY).png
	   @install -Dm644 $(FFQ_RES)/MainLogo32.png $(FFQ_ICONDEST)/32x32/apps/$(FFQ_BINARY).png
	   @install -Dm644 $(FFQ_RES)/MainLogo64.png $(FFQ_ICONDEST)/64x64/apps/$(FFQ_BINARY).png
	   @install -Dm644 $(FFQ_RES)/MainLogo128.png $(FFQ_ICONDEST)/128x128/apps/$(FFQ_BINARY).png
	   @install -Dm644 $(FFQ_RES)/MainLogo256.png $(FFQ_ICONDEST)/256x256/apps/$(FFQ_BINARY).png
	   @install -Dm644 $(FFQ_RES)/MainLogo512.png $(FFQ_ICONDEST)/512x512/apps/$(FFQ_BINARY).png
	   @install -Dm755 $(FFQ_RES)/$(FFQ_BINARY).desktop $(FFQ_DEST)/applications/$(FFQ_BINARY).desktop
	 @else
	   @xdg-icon-resource install --novendor --size 16 $(FFQ_RES)/MainLogo16.png $(FFQ_BINARY)
	   @xdg-icon-resource install --novendor --size 32 $(FFQ_RES)/MainLogo32.png $(FFQ_BINARY)
	   @xdg-icon-resource install --novendor --size 64 $(FFQ_RES)/MainLogo64.png $(FFQ_BINARY)
	   @xdg-icon-resource install --novendor --size 128 $(FFQ_RES)/MainLogo128.png $(FFQ_BINARY)
	   @xdg-icon-resource install --novendor --size 256 $(FFQ_RES)/MainLogo256.png $(FFQ_BINARY)
	   @xdg-icon-resource install --novendor --size 512 $(FFQ_RES)/MainLogo512.png $(FFQ_BINARY)
	   @xdg-desktop-menu install --novendor $(FFQ_RES)/$(FFQ_BINARY).desktop
	   @FFQ_DF=$(FFQ_DEST)/applications/$(FFQ_BINARY).desktop
	   @if [ -f "$$FFQ_DF" ]; then chmod 755 "$$FFQ_DF"; fi
	 @fi

.ONESHELL:
uninstall-hook:
	 @if [ -z "$(FFQ_XDGVER)" ]; then
	   @rm -f $(FFQ_ICONDEST)/16x16/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_ICONDEST)/32x32/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_ICONDEST)/64x64/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_ICONDEST)/128x128/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_ICONDEST)/256x256/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_ICONDEST)/512x512/apps/$(FFQ_BINARY).png
	   @rm -f $(FFQ_DEST)/applications/$(FFQ_BINARY).desktop
	 @else
	   @xdg-icon-resource uninstall --novendor --size 16 $(FFQ_BINARY)
	   @xdg-icon-resource uninstall --novendor --size 32 $(FFQ_BINARY)
	   @xdg-icon-resource uninstall --novendor --size 64 $(FFQ_BINARY)
	   @xdg-icon-resource uninstall --novendor --size 128 $(FFQ_BINARY)
	   @xdg-icon-resource uninstall --novendor --size 256 $(FFQ_BINARY)
	   @xdg-icon-resource uninstall --novendor --size 512 $(FFQ_BINARY)
	   @xdg-desktop-menu uninstall --novendor $(FFQ_BINARY).desktop
	 @fi
