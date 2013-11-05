
CREATE TABLE GRAPHE (identifiant VARCHAR(25) NOT NULL PRIMARY KEY,
                    oriente bool,
                    pondere bool
                   )ENGINE=InnoDB;




CREATE TABLE NOEUD (id INT PRIMARY KEY AUTO_INCREMENT,
                   id_noeud INT,
                   node_value float,
                   x INT,
                   y INT,
                   identifiant VARCHAR(25) NOT NULL
          check(id_node>0),
          FOREIGN KEY (identifiant) REFERENCES GRAPHE(identifiant) ON DELETE CASCADE
                  )ENGINE=InnoDB;




CREATE TABLE ARC (id_arc int PRIMARY KEY AUTO_INCREMENT,
                   valeur float,
                   noeud_aller int NOT NULL,
                   noeud_retour int NOT NULL,
          check(aller>0),
          check(retour>0),
          FOREIGN KEY (noeud_aller) REFERENCES NOEUD(id) ON DELETE CASCADE,
          FOREIGN KEY (noeud_retour) REFERENCES NOEUD(id) ON DELETE CASCADE
                  )ENGINE=InnoDB;