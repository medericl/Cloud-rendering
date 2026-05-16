opti: Mais si tu augmentes juste ça, ça va coûter cher. Donc ensuite:

ajoute du jitter sur le premier sample pour casser les bandes; utilise early
exit, que tu as déjà avec transmittance <= 0.001f; fais moins de LIGHT_STEPS
que de FOG_STEPS, par exemple 6 à 12; plus tard: rendu demi-résolution +
reprojection temporelle.  L’article arXiv sur les optimisations de cloudscapes
parle justement de réduire les steps avec jitter + anti-aliasing temporel.

Priorité 5: Revoir la forme générale du nuage avec une coverage map Ton
CLOUD_COVERAGE est global: src/scene/config.hh (line 46) cpp

inline float CLOUD_COVERAGE = 1.5;

Dans les jeux, on utilise souvent une carte 2D de couverture/météo: selon (x,
z), tu contrôles s’il y a des nuages, quel type, quelle hauteur, quelle
densité. Même une simple FBM 2D ferait déjà beaucoup: cpp

float coverage = fbm(Point3(p.x * 0.01f, 0.0f, p.z * 0.01f), 4);

Puis tu l’utilises pour ouvrir/fermer les masses nuageuses. Ça évite l’effet
“un seul gros bloc de fumée dans une boîte”.


Priorité 6: Corriger le modèle
couleur fog_color * accum peut facilement devenir non physique: plus la densité
augmente, plus tu ajoutes une couleur fixe. Un nuage réaliste devrait plutôt
accumuler de la lumière diffusée: cpp



Color result = background * transmittance + scatteredLight;

Pas juste: cpp



background * transmittance + cloudColor * densityIntegral;

Ça rejoint la priorité 1.  Priorité 7: Atmosphère et horizon Actuellement ton
background_color est une couleur fixe. Pour un rendu “jeu”, il faut au minimum
un ciel dépendant de la direction du rayon: plus clair vers l’horizon, plus
bleu en haut, plus chaud vers le soleil. Même un sky gradient simple améliorera
énormément l’intégration des nuages.


Extraire cloud_density(p).
Passer accum de float à Color.
Ajouter SUN_DIRECTION, SUN_COLOR, AMBIENT_SKY_COLOR.
Ajouter lightTransmittance(p, sunDir).
Ajouter hg_phase.
Monter FOG_STEPS vers 48/64 et ajouter jitter.
Ajouter une coverage map 2D.
Plus tard seulement: reprojection temporelle, half-res rendering, blue noise, shadow map volumétrique.

