// 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
int bot[9] = { 7, 10, 16, 27, -2, -2, -2, -2, -2 };
int player[9] = { -2, -2, -2, -2, -2, -2, -2, -2, -2 };
int breaking_news = true;           // variable qui s'active après que le joueur est déplacé sa propre piece
int choix[4] = { -2, -2, -2, -2 };  // fait la liste du nombre de choix que nous pouvons prendre

void setup() {
  Serial.begin(9600);  // Initialisation du moniteur sériel
  Serial.println("start");
}

void option_move(int i) {  //détermine les choix de move possibles, selon un i move

  //réinitialiser la variable choix:
  for (int j = 0; j <= 3; j++) {
    choix[j] = -2;
  }
  ///////////calcul des choix pour sup et inf///////////
  //boucle sup:
  int move_choix = bot[i] + 1;  // déterminer le move pour la boucle (devant)

  move_choix = trop_grand(move_choix);

  int meme_position = case_occupe(move_choix);  // fonction qui vérifie si la case est occcupé

  if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
    choix[0] = move_choix;
  }

  //boucle inf:
  move_choix = bot[i] - 1;  // déterminer le move pour la boucle (derriere)
  move_choix = trop_petit(move_choix);

  meme_position = case_occupe(move_choix);

  if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
    choix[1] = move_choix;
  }

  /////////Calcul des choix inter-boucle///////////
  // 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
  for (int j = 0; j <= 2; j++) {                                                                                                 // pour optimiser le stockage: nous allons utiliser la boucle pour prendre toutes les possibilités en compte
    if ((bot[i] == (1 + (j * 10))) || (bot[i] == (3 + (j * 10))) || (bot[i] == (5 + (j * 10))) || (bot[i] == (7 + (j * 10)))) {  //ajoute la case dans la liste des choix qui peuvent être prises
      if (j != 2) {                                                                                                              // pour ne pas faire + 10 pour la  boucle intérieur (car il n'y a pas plus )
        meme_position = case_occupe(bot[i] + 10);
        if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
          choix[2] = bot[i] + 10;
        }
      }
      // ne peut pas mettre else if, sinon la boucle centrale ne sera pas comptabilisé
      if (j != 0) {  // pour ne pas faire - 10 pour la  boucle exterieur (car il n'y a pas moins )
        meme_position = case_occupe(bot[i] - 10);
        if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
          choix[3] = bot[i] - 10;
        }
      }
    }
  }
}

int trop_grand(int move) {
  // 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
  if ((move == 8) || (move == 18) || (move == 28)) {  //permet de boucler la boucle pour les boucles du moulin (qd trop haut)
    move = move - 8;
  }
  return move;
}
int trop_petit(int move) {
  // 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
  if ((move == -1) || (move == 9) || (move == 19)) {  //permet de boucler la boucle pour les boucles du moulin (qd trop bas)
    move = move + 8;
  }
  return move;
}
int case_occupe(int move_choix) {  //vérifie si la case est déjà prise ou pas
  int meme_position = false;
  for (int j = 0; j <= 8; j++) {  // verifie si la case est deja prise
    if ((move_choix == bot[j]) || (move_choix == player[j])) {
      meme_position = true;
    }
  }
  return meme_position;
}
int alignement(int step_one, int step_two, int killing) {
  int commun = 0;
  for (int l = 0; l <= 8; l++) {
    if ((bot[l] == step_one) || (bot[l] == step_two)) {
      commun = commun + 1;
      if (commun == 2) {
        killing = true;
      }
    }
  }
  return killing;
}
int global_alignement(int choix_premier) {  // permet de voir si ce move joué résulte en un alignement
  int killing = false;                      // détermine si le bot peut enlever une piece adverse


  if (choix_premier % 2 == 0) {                    //les nombres pairs se trouvent au coin du moulin
    int step_one = trop_grand(choix_premier + 1);  // définir les cases d'un premier alignement
    int step_two = trop_grand(step_one + 1);
    Serial.println("killing_corner");
    Serial.println(step_one);
    Serial.println(step_two);

    killing = alignement(step_one, step_two, killing);

    step_one = trop_petit(choix_premier - 1);  // définir les cases d'un second alignement
    step_two = trop_petit(step_one - 1);
    Serial.println("killing_corner_2");
    Serial.println(step_one);
    Serial.println(step_two);
    killing = alignement(step_one, step_two, killing);

  } else {

    // correpond aux nombres impairs se trouvant à une intersection
    //pour un aliniement se trouvan sur un ring:
    int step_one = trop_grand(choix_premier + 1);  // définir les cases d'un premier alignement
    trop_grand(step_one);
    int step_two = trop_grand(choix_premier - 1);
    trop_petit(step_two);
    Serial.println("killing_middle");
    Serial.println(step_one);
    Serial.println(step_two);
    killing = alignement(step_one, step_two, killing);

    //pour un alignement inter-ring:
    int case_deplace = choix_premier;
    int alignement_inter = -2;
    for (int l = 0; l <= 2; l++) {          // détermine sur quelle rangée inter-ring se trouve l'aliniement potentiel
      for (int m = 1; m <= 7; m = m + 2) {  //détermine si la case déplacé est celle de la case 1, 3, 5 ou 7; si ce n'est pas le cas, on soustraie tout par 10
        if (case_deplace == m) {
          alignement_inter = m;
        }
      }
      case_deplace = case_deplace - 10;
    }

    // génère les steps:
    step_one = -1;     // pour que le bout de code apres fonctionne //TRES IMPORTANT QU CA SOIT -1 et pas -2 Pour la section plus tard
    int first = true;  // premiere fois que l'element passe par la boucle
    for (int l = 0; l <= 2; l++) {
      if (alignement_inter + l * 10 != choix_premier) {
        if (first == true) {
          step_one = alignement_inter + l * 10;
          first = false;

        } else {
          step_two = alignement_inter + l * 10;
        }
      }
    }
    Serial.println("killing_middle_2");
    Serial.println(step_one);
    Serial.println(step_two);
    killing = alignement(step_one, step_two, killing);
  }
  if (killing == true) {
    return true;

  } else {
    return false;
  }
}



void loop() {
  delay(50);

  int in_beginning = false;
  if (breaking_news == true) {   // le tour où on a actualisé le move du joueur
    if (in_beginning == true) {  //en debut de jeu qd les joueurs place les pieces

      //options pour placer une piece:
      for (int i = 0; i <= 27; i++) {
        if ((case_occupe(i) == false) && (i != 8) && (i != 9) && (i != 18) && (i != 19)) {

          //voir si aliniement

          int killing = global_alignement(i);


          //si killing == true; déterminer un des pions qu'on peut enlever (pour +2) :



          //voir si on arrive a bloquer l'adversaire pour aire un alignement (pour +1)
        }
      }

    } else {
      for (int i = 0; i <= 8; i++) {
        if (bot[i] != -2) {  // permet d'enlever les cases qu'on a pas
                             //////détermine les choix de move possibles, selon une pièce se trouvant sur la case i//////////
          option_move(i);

          Serial.println("f");
          Serial.println(choix[0]);
          Serial.println(choix[1]);
          Serial.println(choix[2]);
          Serial.println(choix[3]);

          int sauvegarde_i = bot[i];  // sauvegarder le changement qui va etre effectué juste apres
          int choix_primaire[4];
          for (int j = 0; j <= 3; j++) {   // sauvegarder les moves possibles pour ce pion
            choix_primaire[j] = choix[j];  //primaire --> la liste des choix genere pour ce pion i
          }


          for (int j = 0; j <= 3; j++) {
            if (choix_primaire[j] != -2) {
              //////regarde quel changement apporte chacun des move de la case i//////////
              bot[i] = choix_primaire[j];  // temporairement modifier la liste bot pour voir un move à l'avance
              Serial.println("move:");
              Serial.println(sauvegarde_i);
              Serial.println("-->");
              Serial.println(bot[i]);

              ////////déterminer si on peut enlever un pion adversaire apres notre move:////////////
              //Rappel: il est obligé que ca soit CE move du bot qui permet d'aligner les pions, donc on a cas regarder les moves possibles autour de se pion qui a été déplacé
              // (suite) puis on regarde si les cases alignés sont occupés par les pions du bot, si c'est le cas, alors il peut enelver un pion adverse! Car il vient tout juste de faire une "alinéation"!

              //déterminer si on peut faire un alignement (pour +2) (car on enleve la piece d'un adversaire et on peut potentiellement enlever un moulin adverse):
              int killing = global_alignement(choix_primaire[j]);

              if (killing == true) {
                Serial.println("kill");
                //////déterminer la meilleure piece à prendre, en déterminant toutes les possibilité de jeu de l'adversaire après avoir enlevé une de ses pieces//////
                //déterminer si l'adversaire peut faire un alignement
                if (killing_2 == true) {
                  Serial.println("kill_2");
                  //////déterminer la meilleure piece à prendre, en déterminant toutes les possibilité de jeu du robot après avoir enlevé une de ses pieces//////
                  //////JUSTE EN REGARDANT LES MOVES QUE le robot POURRA ALIGNER OU BLOQUER PAR LA SUITE


                  ////faire evaluation

                } else {
                  Serial.println("no kill_2");

                  //////JUSTE EN REGARDER LES MOVES QUE le robot POURRA ALIGNER OU BLOQUER PAR LA SUITE

                  ////faire evaluation
                }

              } else {
                Serial.println("no kill");
                //déterminer si l'adversaire peut faire un alignement
                if (killing_2 == true) {
                  Serial.println("kill_2");
                  //////déterminer la meilleure piece à prendre, en déterminant toutes les possibilité de jeu du robot après avoir enlevé une de ses pieces//////
                  //////JUSTE EN REGARDANT LES MOVES QUE le robot POURRA ALIGNER OU BLOQUER PAR LA SUITE


                  ////faire evaluation

                } else {
                  Serial.println("no kill_2");

                  //////JUSTE EN REGARDER LES MOVES QUE le robot POURRA ALIGNER OU BLOQUER PAR LA SUITE

                  ////faire evaluation
                }
              }



              //si on peut enlever une piece, quel est la meilleure à prendre, selon le move que le joueur apres pourrait faire



              //voir si on arrive a bloquer l'adversaire, si killing == false (pour +1):




              //a ce stade, le score de tous les move est enregistré
              //MAIS, si il y a un score de 0, alors on enregistrerat seulement les 0 dorenavant
              //MAIS, si il y a un score de +1, alors on enregistrerat seulement les +1 dorenavant
              //parce que comme nous faisons un depth de 3, meme si un move de score 0 devient +1, et bien il va falloir tout de même determiner aleatoirement le meilleur move #optimisation + le meilleur move le plus soon
              //la même chose s'applqiue si on a que des move de -1 ou 0
              //tout cela est possible car nous sommes en depth de 3 (sinon il aurait  directement fallu evaluer notre move possible à nous )
            }
          }


          bot[i] = sauvegarde_i;  //remettre la liste comme elle l'était

          //Si il y a plusieurs choix de move qui ont le meme score, alors aller au cas par cas (en ayant enregistrer notre premier move et les move possible de notre adversaire qui serait engendré)
          // voir un step dans le futur en plus
          // pour determiner si un move nous permettrait d'enlever un pion de l'adversaire
          //si c'est egal, avoir fait une moyenne des points
          //sinon faire un move random
        }
      }


      breaking_news = false;  //fait en sorte que le programme minmax ne s'active pas une seconde fois
    }
  }
}
