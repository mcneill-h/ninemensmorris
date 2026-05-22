// 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
int bot[9] = { -2, 7, 13, -2, -2, 11, 15, -2, -2 };
int player[9] = { -2, -2, -2, -2, 25, 1, -2, -2, -2 };
int breaking_news = true;  // variable qui s'active après que le joueur est déplacé sa propre piece

void setup() {
  Serial.begin(9600);  // Initialisation du moniteur sériel
}

void loop() {
  delay(50);

  if (breaking_news == true) {  // le tour où on a actualisé le move du joueur
    for (int i = 0; i <= 8; i++) {
      if (bot[i] != -2) {  // permet d'enlever les ports qu'on a pas
        int choix[4] = { -2, -2, -2, -2 }; // fait la liste du nombre de choix que nous pouvons prendre 
        
        ///////////calcul des choix pour sup et inf///////////
        //boucle sup:
        int move_choix = bot[i] + 1;  // déterminer le move pour la boucle (devant)

        // 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
        if ((move_choix == 8) || (move_choix == 18) || (move_choix == 28)) {  //permet de boucler la boucle pour les boucles du moulin (qd trop haut)
          move_choix = move_choix - 8;
        }
        int meme_position = case_occupe(move_choix);  // fonction qui vérifie si la case est occcupé

        if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
          choix[0] = move_choix;
          
        }

        //boucle inf:
        move_choix = bot[i] - 1;  // déterminer le move pour la boucle (derriere)
        // 0-7: ring exterieur // 10-17 ring central // 20-27 ring interieur
        if ((move_choix == -1) || (move_choix == 9) || (move_choix == 19)) {  //permet de boucler la boucle pour les boucles du moulin (qd trop bas)
          move_choix = move_choix + 8;
        }

        meme_position = case_occupe(move_choix);

        if (meme_position == false) {  //ajoute la case dans la liste des choix qui peuvent être prises
          choix[1] = move_choix;
        }
        //Serial.println("e");
        //Serial.println(choix[0]);
        //Serial.println(choix[1]);

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
        //Serial.println("f");
        //Serial.println(choix[0]);
        //Serial.println(choix[1]);
        //Serial.println(choix[2]);
        //Serial.println(choix[3]);

        ////////Si on fait ce move, qu'est ce que l'adversaire pourrait faire:///////////
        for (int j = 0; j <= 3; j++) { 
          if (bot[j]!=-2) {
            int possibilite[9] = {bot[0],bot[1],bot[2],bot[3],bot[4],bot[5],bot[6],bot[7],bot[8]};
            possibilite[i] = choix[j];




          }

        }



      }
    }
  }
  
  breaking_news = false;  //fait en sorte que le programme minmax ne s'active pas une seconde fois
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
int option_move() {









}
