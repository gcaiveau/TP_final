# TP_final

Ajout : 3 niveaux de difficulté

	Niveau 1 (mode "Débutant"):
		Mode EasyPeasy : moins de notes proches les unes des autres
		aucun multiplicateur de score (no pain, no gain)
		Imprécision autorisée plus grande (0.2f)
		Temps de chute des notes plus long (1.3*2)
		
	Niveau 2 (mode "Normal"):
		Plus de mode EasyPeasy à partir de ce niveau
		Multiplicateur de score +1.2 au lieu de +1(multiplicateur "nul" du premier niveau)
		Imprécision moins tolérée (0.15f)
		Temps de chute des notes plus cours (1*2)

	Niveau 3 (mode "Hardcore"):
		Multiplicateur de score +1.5 au lieu de +1(multiplicateur "nul" du premier niveau)
		Imprécision très peu tolérée (0.07)
		Temps de chute des notes très cours (0.7*2)
		Mode NoMistakes : si une touche est pressée alors qu'elle ne devrait pas l'être le score diminue d'un point

Ajout : 
	Note parfaite, si une note est jouée avec une imprécision inférieure à 0.01, compte double pour le score et pour le combo
	

		
		
		
		
		