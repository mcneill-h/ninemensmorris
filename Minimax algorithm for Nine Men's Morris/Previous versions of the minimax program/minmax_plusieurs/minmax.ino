// 1-8: ring exterieur // 11-18 ring central // 21-28 ring interieur
int bot[9] = { -1, -1, -1, -1, -1 , -1, -1, -1, -1 };
int player[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int breaking_news = true;  // variable qui s'active après que le joueur est déplacé sa propre piece

void setup() {
  Serial.begin(9600);  // Initialisation du moniteur sériel

}

void loop() {
  delay(50);

  if (breaking_news == true) { // le tour où on a actualisé le move du joueur 
    for (int i = 0; i <=8 ; i++) {
      if (bot[i]!=-1) { // permet d'enlever les ports qu'on a pas 
        int choix[9] = { -1, -1, -1, -1};
        

        
      }
    }
    breaking_news = false;
  }

// 1-8: ring exterieur // 11-18 ring central // 21-28 ring interieur
int news = 27;             // l'emplacement ou la nouvelle piece a ete deplacé
  if ((news == 9) || (news == 19) || (news == 29)) {  //permet de boucler la boucle pour les boucles du moulin
    news = news - 8;
  }
  else if ((news == 0) || (news == 10) || (news == 20)) {  //permet de boucler la boucle pour les boucles du moulin
    news = news + 8;
  }

}
