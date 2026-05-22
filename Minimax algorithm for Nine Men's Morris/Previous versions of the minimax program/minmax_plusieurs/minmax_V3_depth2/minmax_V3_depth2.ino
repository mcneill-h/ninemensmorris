// 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
int bot[9] = { 6, -2, 25, 24, 20, -2, 3, 4, 11 };  //{ -2, 17, 27, 1, 11, -2, -2, -2, -2 }
int taille_liste_bot = 8;                            //donne le nombre d'éléments dans la liste bot

int player[9] = { 5, 15, 16, 26, 23, 0, 13, 2, 1 };  //{ 3, 2, 14, 15, -2, -2, -2, -2, -2 }
int breaking_news = true;                               // variable qui s'active après que le joueur est déplacé sa propre piece
int choix[4] = { -2, -2, -2, -2 };                      // fait la liste du nombre de choix que nous pouvons prendre
int liste_option_move[9] = { -2, -2, -2, -2, -2, -2, -2, -2, -2 };
int liste_global_alignement[9] = { -2, -2, -2, -2, -2, -2, -2, -2, -2 };


////changements finaux///
int move_optimal_case_depart;  //définit la position de l'élément dans la liste qui va etre modifié
int move_optimal_case_arrive;  //définit avec quoi on va remplacer
int piece_a_suprime_final;     //definit la position de la piece que nous devons suprimer pour minimiser le score adverse (seulement si on peut kill)
int score_final = -100;        // aide à déterminer le score final de ce move


void setup() {
  Serial.begin(9600);  // Initialisation du moniteur sériel
  Serial.println("-------------start-------------");
  randomSeed(analogRead(A4));
}

void option_move(int i) {  //détermine les choix de move possibles, selon un i move

  //réinitialiser la variable choix:
  for (int j = 0; j <= 3; j++) {
    choix[j] = -2;
  }
  ///////////calcul des choix pour sup et inf///////////
  //boucle sup:
  int move_choix = liste_option_move[i] + 1;  // déterminer le move pour la boucle (devant)

  move_choix = trop_grand(move_choix);

  int meme_position = case_occupe(move_choix);  // fonction qui vérifie si la case est occcupé

  if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
    choix[0] = move_choix;
  }

  //boucle inf:
  move_choix = liste_option_move[i] - 1;  // déterminer le move pour la boucle (derriere)
  move_choix = trop_petit(move_choix);

  meme_position = case_occupe(move_choix);

  if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
    choix[1] = move_choix;
  }

  /////////Calcul des choix inter-boucle///////////
  // 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
  for (int j = 0; j <= 2; j++) {                                                                                                                                                         // pour optimiser le stockage: nous allons utiliser la boucle pour prendre toutes les possibilités en compte
    if ((liste_option_move[i] == (1 + (j * 10))) || (liste_option_move[i] == (3 + (j * 10))) || (liste_option_move[i] == (5 + (j * 10))) || (liste_option_move[i] == (7 + (j * 10)))) {  //ajoute la case dans la liste des choix qui peuvent être prises
      if (j != 2) {                                                                                                                                                                      // pour ne pas faire + 10 pour la  boucle intérieur (car il n'y a pas plus )
        meme_position = case_occupe(liste_option_move[i] + 10);
        if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
          choix[2] = liste_option_move[i] + 10;
        }
      }
      // ne peut pas mettre else if, sinon la boucle centrale ne sera pas comptabilisé
      if (j != 0) {  // pour ne pas faire - 10 pour la  boucle exterieur (car il n'y a pas moins )
        meme_position = case_occupe(liste_option_move[i] - 10);
        if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
          choix[3] = liste_option_move[i] - 10;
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
    if ((liste_global_alignement[l] == step_one) || (liste_global_alignement[l] == step_two)) {
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


    killing = alignement(step_one, step_two, killing);

    step_one = trop_petit(choix_premier - 1);  // définir les cases d'un second alignement
    step_two = trop_petit(step_one - 1);

    killing = alignement(step_one, step_two, killing);

  } else {

    // correpond aux nombres impairs se trouvant à une intersection
    //pour un aliniement se trouvan sur un ring:
    int step_one = trop_grand(choix_premier + 1);  // définir les cases d'un premier alignement
    trop_grand(step_one);
    int step_two = trop_grand(choix_premier - 1);
    trop_petit(step_two);


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

    killing = alignement(step_one, step_two, killing);
  }
  if (killing == true) {
    return true;

  } else {
    return false;
  }
}


int move_adversaire() {  // DETERMINER LES MOVES POSSIBLES DE L'ADVERSAIRE
  int resultat = 0;      // permet d'enregistrer le score final du move
  for (int l = 0; l <= 8; l++) {
    if (player[l] != -2) {

      for (int m = 0; m <= 8; m++) {  // remplacer liste_option_move par la liste de player
        liste_option_move[m] = player[m];
      }
      option_move(l);  //déterminer les possibilite de move pour chaque pion adverse

      int sauvegarde_l = player[l];

      int killing_2 = false;
      int blocage_2 = false;
      for (int m = 0; m <= 3; m++) {  //si on applique ce move spécifique, y a t'il un alignement ou un blocage?
        if (choix[m] != -2) {
          player[l] = choix[m];




          //élim?
          for (int n = 0; n <= 8; n++) {  // remplacer liste_global_alignement par la liste de player
            liste_global_alignement[n] = player[n];
          }
          if (global_alignement(choix[m]) == true) {
            killing_2 = true;

            Serial.println("retour_kill:");
            Serial.println(sauvegarde_l);
            Serial.println("-->");
            Serial.println(choix[m]);
          }


          //blocage?
          for (int n = 0; n <= 8; n++) {  // remplacer liste_global_alignement par la liste de player
            liste_global_alignement[n] = bot[n];
          }

          if (global_alignement(choix[m]) == true) {

            blocage_2 = true;
            Serial.println("retour_blocage:");
            Serial.println(sauvegarde_l);
            Serial.println("-->");
            Serial.println(choix[m]);
          }
        }
      }
      player[l] = sauvegarde_l;

      if (killing_2 == true) {
        return -2;  // car  - 1(perdre un pion à nous) = 0
        // on return directement car il ne peut pas y avoir de pire possibilité (pas besoin d'utiliser la variable "resultat")

      } else if (blocage_2 == true) {
        resultat = -1;  // car  - 0.5 (ne plus pouvoir faire de moulin) = 0.5
      }
    }
  }
  return resultat;  // car on ne perd rien
}

void changements_finaux(int i, int choix_primaire, int score, int piece_a_suprime) {
  move_optimal_case_depart = i;               //position dans la liste où la case doit être changée
  move_optimal_case_arrive = choix_primaire;  // par quelle case doit on changer
  score_final = score;
  piece_a_suprime_final = piece_a_suprime;  // sera egal à -2 s'il n'y a pas de killing
}

void loop() {  ////////////////////////////////////////////LOOP//////////////////////////////////////////////
  delay(50);
  score_final = -100;  // aide à déterminer le score final de ce move
  piece_a_suprime_final = -2;

  // égal à -100 pour que il soit directement remplacé

  int in_beginning = false;
  if (breaking_news == true) {   // le tour où on a actualisé le move du joueur
    if (in_beginning == true) {  //en debut de jeu qd les joueurs place les pieces

      //options pour placer une piece:
      for (int i = 0; i <= 27; i++) {
        if ((case_occupe(i) == false) && (i != 8) && (i != 9) && (i != 18) && (i != 19)) {
          int score = 0;

          //voir si aliniement
          for (int j = 0; j <= 8; j++) {  // remplacer liste_global_alignement par la liste de bot

            liste_global_alignement[j] = bot[j];
          }
          int killing = global_alignement(i);

          int piece_a_suprime = -2;  // va changer seulement si killing == true



          //si killing == true; déterminer un des pions qu'on peut enlever:

          if (killing == true) {
            score = score + 10;  //nettement plus haut pour que le kill soit toujours plus important que le bloc

            bot[taille_liste_bot] = i;  // temporairement changer le contenu de la liste

            int score_move_adversaire = -100;  //score adverse obtenu apres notre move
                                               // -100 pour que peut importe le move, score_move_adversaire sera remplacé

            Serial.println("move:");
            Serial.println(i);

            int piece_clef_killing;         // utiliser pour s'assurer que le bot ne panique pas et face pas le mauvais move
            for (int k = 0; k <= 8; k++) {  // pour toutes les possibilité où on suprime un des pions adverse
              if (player[k] != -2) {


                Serial.println("pion supprimé");
                Serial.println(player[k]);

                int sauvegarde_k = player[k];  // sauvegarder le pion qui va être enlevé



                player[k] = -2;

                int score_challenge_worst = 100;  //enregistre le pire score que l'adversaire peut faire si nous surpimons cette piece
                for (int l = 0; l <= 27; l++) {   //move possible de l'adversaire

                  if ((case_occupe(l) == false) && (l != 8) && (l != 9) && (l != 18) && (l != 19)) {
                    int score_challenge = 0;
                    //voir si aliniement
                    for (int m = 0; m <= 8; m++) {  // remplacer liste_global_alignement par la liste de bot
                      liste_global_alignement[m] = player[m];
                    }
                    if (global_alignement(l) == true) {
                      score_challenge = -2;
                    }

                    //voir si blocage
                    for (int m = 0; m <= 8; m++) {  // remplacer liste_global_alignement par la liste de bot
                      liste_global_alignement[m] = bot[m];
                    }
                    if (global_alignement(l) == true) {
                      score_challenge = score_challenge - 1;
                    }



                    //garder le pire score possible pour nous
                    if (score_challenge < score_challenge_worst) {  // l'adversaire maximise notre perte
                      score_challenge_worst = score_challenge;
                    }
                  }
                }

                Serial.println("score");
                Serial.println(score_challenge_worst);

                if (score_move_adversaire < score_challenge_worst) {
                  score_move_adversaire = score_challenge_worst;

                  piece_a_suprime = k;
                }


                player[k] = sauvegarde_k;
              }
            }


            score = score + score_move_adversaire;


            bot[taille_liste_bot] = -2;
          }


          //voir si on arrive a bloquer l'adversaire pour évitier un alignement
          for (int j = 0; j <= 8; j++) {  // remplacer liste_global_alignement par la liste de player
            liste_global_alignement[j] = player[j];
          }
          int blocage = global_alignement(i);

          if (blocage == true) {
            score = score + 1;
          }



          if (score > score_final) {
            changements_finaux(taille_liste_bot, i, score, piece_a_suprime);
          } else if (score == score_final) {
            if (random(1, 15) == 1) { //on choisit random 
            
              changements_finaux(taille_liste_bot, i, score, piece_a_suprime);
            }
          }
        }
      }

      //score final
      Serial.println("changement final:");
      Serial.println(bot[move_optimal_case_depart]);
      Serial.println("-->");
      Serial.println(move_optimal_case_arrive);


      Serial.println("position de la piece a suprime:");
      Serial.println(piece_a_suprime_final);

      Serial.println("score");
      Serial.println(score_final);

      bot[move_optimal_case_depart] = move_optimal_case_arrive;
      if (piece_a_suprime_final != -2) {

        player[piece_a_suprime_final] = -2;
      }


      breaking_news = false;
      taille_liste_bot = taille_liste_bot + 1;

    } else {


      for (int i = 0; i <= 8; i++) {  //pour tout move se trouvant à la case i de la liste "bot"
        if (bot[i] != -2) {           // permet d'enlever les cases qu'on a pas
                                      //////détermine les choix de move possibles, selon une pièce se trouvant sur la case i//////////

          for (int j = 0; j <= 8; j++) {  // remplacer liste_option_move par la liste de bot
            liste_option_move[j] = bot[j];
          }
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


          for (int j = 0; j <= 3; j++) {  //déterminer le score d'un move (si on deplace a-->b (par exemple))
            if (choix_primaire[j] != -2) {
              int piece_a_suprime = -2;  //enregistre la position de la piece a surpimer
              int score = 0;             // aide à déterminer le score final de ce move
                                         // s'active suelement si le bot peut kill une piece adverse

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
              for (int k = 0; k <= 8; k++) {  // remplacer liste_global_alignement par la liste de bot
                liste_global_alignement[k] = bot[k];
              }
              int killing = global_alignement(choix_primaire[j]);




              if (killing == true) {
                score = 2;  // aide à déterminer le score final du move, +2 car killing est poisitif


                Serial.println("kill");
                //////déterminer la meilleure piece à prendre, en déterminant toutes les possibilité de jeu (alignement ou blocage) de l'adversaire après avoir enlevé une de ses pieces//////


                int score_move_adversaire = -100;  //score adverse obtenu apres notre move
                // -100 pour que peut importe le move, score_move_adversaire sera remplacé

                for (int k = 0; k <= 8; k++) {  // pour toutes les possibilité où on suprime un des pions adverse
                  if (player[k] != -2) {

                    Serial.println("élim:");
                    Serial.println(player[k]);
                    int sauvegarde_k = player[k];  // sauvegarder le pion qui va être enlevé

                    player[k] = -2;

                    int score_challenge = move_adversaire();

                    if (score_move_adversaire < score_challenge) {

                      piece_a_suprime = k;  //enregistre la position de la piece a surpimer
                      score_move_adversaire = score_challenge;
                    }


                    player[k] = sauvegarde_k;
                  }
                }


                score = score + score_move_adversaire;
                Serial.println("score");
                Serial.println(score);
                Serial.println("piece_a_suprime");
                Serial.println(player[piece_a_suprime]);


              } else {
                Serial.println("no kill");
                //déterminer si l'adversaire peut faire un alignement ou bloquer


                int score_move_adversaire = move_adversaire();
                score = score + score_move_adversaire;
                Serial.println("score");
                Serial.println(score);
              }


              //faire evalution final pour la prochaine boucle:

              if (score > score_final) {

                changements_finaux(i, choix_primaire[j], score, piece_a_suprime);


              } else if (score == score_final) {                                // si on a un score egal
                if ((piece_a_suprime != 2) && (piece_a_suprime_final == -2)) {  //priorité au move qui permet de kill

                  changements_finaux(i, choix_primaire[j], score, piece_a_suprime);

                } else if ((score_final == 0) && (score == 0)) {  //pour activer une machine à killing (depth 3)
                                                                  // si égalité qd il y a "0 ; 0": regarder si le premier move provient d'un alignement deja fait (pour mettre en place la death machine)
                  bot[i] = sauvegarde_i;                          //remettre la liste comme elle l'était

                  for (int k = 0; k <= 8; k++) {  // remplacer liste_global_alignement par la liste de bot
                    liste_global_alignement[k] = bot[k];
                  }

                  if (global_alignement(sauvegarde_i) == true) {

                    changements_finaux(i, choix_primaire[j], score, piece_a_suprime);
                  }
                } else if (random(1, 15) == 1) {////sinon on choisit random 

                  changements_finaux(taille_liste_bot, i, score, piece_a_suprime);
                }

               
              }
            }
          }


          bot[i] = sauvegarde_i;  //remettre la liste comme elle l'était
        }
      }

      ///jouer le move considéré comme optimale
      Serial.println("changement final:");
      Serial.println(bot[move_optimal_case_depart]);
      Serial.println("-->");
      Serial.println(move_optimal_case_arrive);

      Serial.println("position de la piece a suprime:");
      Serial.println(piece_a_suprime_final);

      Serial.println("score_final");
      Serial.println(score_final);


      bot[move_optimal_case_depart] = move_optimal_case_arrive;
      if (piece_a_suprime_final != -2) {

        player[piece_a_suprime_final] = -2;
      }

      breaking_news = false;  //fait en sorte que le programme minmax ne s'active pas une seconde fois
    }
  }
}
