#!/bin/bash
mkdir -p ~/.local/share/icons/hicolor/48x48/apps/
cp ../ui/data/icons/icon_gray.png ~/.local/share/icons/hicolor/48x48/apps/
cp ../ui/data/icons/icon_white.png ~/.local/share/icons/hicolor/48x48/apps/

mkdir -p ~/.local/share/icons/hicolor
cp ../ui/theme/index.theme ~/.local/share/icons/hicolor/

gtk-update-icon-cache ~/.local/share/icons/hicolor 
