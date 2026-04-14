la base:

densité
falloff vertical

2. forme
fbm simple
bruit
animation

3. Sortir du look “mousse”
double noise / érosion simple
forme globale plus contrôlée
soit par masque, soit par blobs/SDF

4. Raffiner
plusieurs couches
Worley
lightmarch léger


FBM simple
Érosion simple avec un 2e bruit
Animation lente
Blobs / forme globale plus jolie
Worley si nécessaire
Lightmarch cheap


faire plusieurs couches
créer un environnement: des montagnes, forets...


creer des avions
