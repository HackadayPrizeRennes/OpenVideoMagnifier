#Système
Pour le moment, le projet n'a été testé que sous Linux (Fedora)

#Dépendances logicielles
Pour tout ce qui concerne la partie capture et traitement de l'image, la bibliothèque opencv est utilisé. Pour l'installer sous Fedora, il suffit d'éxécuter les commandes :
`sudo dnf install opencv opencv-devel -y`
Toute l'interface graphique de l'application est gérée par la bilbiothèque Qt. Pour l'installer : 
`wget http://download.qt.io/official_releases/online_installers/qt-unified-linux-x86-online.run #Ou autre selon votre architecture
chmod +x qt-unified-x64-online.run
sudo ./qt-unified-linux-x64-online.run`
Dans le futur, le projet sera compatible avec plusieurs OCR. Pour le moment, l'OCR choisit est GOCR car c'est celui qui donne les meilleurs résultats. Pour l'installer il suffit d'éxécuter :
`sudo dnf install gocr -y`

