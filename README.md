# OpenVideoMagnifier
The Open Source Video Magnifier project

# English (version française plus bas)

## Objectives
This project is about developing free (as in freedom) imagery software and harware for visually deficient people. The software should be able to interface with video cameras.
### Main Goals:
- Zoom in on text, invert colors, adapt to color blindness.
- provide open hardware schematics for building easy to use generic camera stand hardware.
### additionnal features:
- use Optical Character Recognition (OCR) to read aloud selected text.
- Adapt to already existing specialized cameras
- structural analysis of documents
- external remote control

([example of specialised camera] (http://www.tousergo.com/loupe-basse-vision/2481-teleagrandisseur-connectable-transformer.html))

# Français

## Description
Développer un logiciel libre d'imagerie pour l'aide à la lecture, et la vision en général et d'un support de caméra pliable ([exemple](http://www.tousergo.com/loupe-basse-vision/2481-teleagrandisseur-connectable-transformer.html)). Il sera possible de zoomer sur le texte, inverser les couleurs, ou changer de palette de couleurs accessible via une boite de controle. De plus, un OCR sera fournit. Enfin, le logiciel doit pouvoir s'adapter aux caméras standard.

## Tâches
- [ ] Design du support - Responsable : AmarOk
- [ ] Modélisation du support - Responsable : Indéfini
- [ ] Réalisation de la boite de controle - Responsable : AmarOk ?
- [ ] Partie logicielle - Responsable : AmarOk (consultant archi/OpenCV: Ksamak)
     - [X] Récupération d'une caméra
     - [ ] Compatibilité multi caméras
     - [ ] reverse engineering d'un modèle (selon dispo)
     - [X] Zoom
     - [X] Inversion de couleurs
     - [X] Changement de palette de couleur
     - [ ] Autres algos (contrast intelligent, detection de contours)
     - [ ] Réaliser une archi propre
     - [ ] Réaliser un code propre
     - [ ] Documenter
     - [X] Traductions (Ksamak)
     - [ ] Algorithme de groupement de texte
     - [ ] analyse structurelle de documents
     - [ ] OCR
