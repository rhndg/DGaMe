#include "game_map.h"

game_map::game_map(int n){ //n should be even
	int n2 = n*n;
	int counter = 0;
	for (int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			Vector3* v = new Vector3[4];
			int* neigh = new int[8];
			for (int k=0; k < 8; k++) neigh[k]=-1;
			v[0].set(-n/2,i-n/2,j-n/2);
			v[1].set(-n/2,i-n/2,j-n/2+1);
			v[2].set(-n/2,i-n/2+1,j-n/2+1);
			v[3].set(-n/2,i-n/2+1,j-n/2);
			
			grid g(v,neigh,counter);
			Maps.push_back(g);
			counter++;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Vector3* v = new Vector3[4];
			int* neigh = new int[8];
			for (int k=0; k < 8; k++) neigh[k]=-1;
			v[0].set(i-n/2,j-n/2,-n/2);
			v[1].set(i-n/2,j-n/2+1,-n/2);
			v[2].set(i-n/2+1,j-n/2+1,-n/2);
			v[3].set(i-n/2+1,j-n/2,-n/2);
			
			grid g(v,neigh,counter);
			Maps.push_back(g);
			counter++;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Vector3* v = new Vector3[4];
			int* neigh = new int[8];
			for (int k=0; k < 8; k++) neigh[k]=-1;
			v[0].set(i-n/2,n/2,j-n/2);
			v[1].set(i-n/2,n/2,j-n/2+1);
			v[2].set(i-n/2+1,n/2,j-n/2+1);
			v[3].set(i-n/2+1,n/2,j-n/2);
			
			grid g(v,neigh,counter);
			Maps.push_back(g);
			counter++;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Vector3* v = new Vector3[4];
			int* neigh = new int[8];
			for (int k=0; k < 8; k++) neigh[k]=-1;
			v[0].set(i-n/2,n/2-j,n/2);
			v[1].set(i-n/2,n/2-j-1,n/2);
			v[2].set(i-n/2+1,n/2-j-1,n/2);
			v[3].set(i-n/2+1,n/2-j,n/2);
			
			grid g(v,neigh,counter);
			Maps.push_back(g);
			counter++;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Vector3* v = new Vector3[4];
			int* neigh = new int[8];
			for (int k=0; k < 8; k++) neigh[k]=-1;
			v[0].set(n/2,n/2-i,j-n/2);
			v[1].set(n/2,n/2-i-1,j-n/2);
			v[2].set(n/2,n/2-i-1,j-n/2+1);
			v[3].set(n/2,n/2-i,j-n/2+1);
			
			grid g(v,neigh,counter);
			Maps.push_back(g);
			counter++;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Vector3* v = new Vector3[4];
			int* neigh = new int[8];
			for (int k=0; k < 8; k++) neigh[k]=-1;
			v[0].set(n/2-i,-n/2,j-n/2);
			v[1].set(n/2-i-1,-n/2,j-n/2);
			v[2].set(n/2-i-1,-n/2,j-n/2+1);
			v[3].set(n/2-i,-n/2,j-n/2+1);
	
			grid g(v,neigh,counter);
			Maps.push_back(g);
			counter++;
		}
	}

	for (int i = 0; i < 6*n2 ; i++){
		grid current = Maps[i];
		int counter = 0;
		for (int j = 0; j < 6*n2 ; j++){
			if (i==j) continue;
			grid search = Maps[j];
			bool break1 = false;
			bool found=false;
			for (int k = 0; k < 4; k++){
				for (int l = 0; l < 4; l++){
					if (current.vertex[k].equal(search.vertex[l])){
						for(int m = 0; m < 8; m++){
							if (Maps[i].neigh[m]==j) found=true;
							break;
						}
						if(found) break;
						else{
							Maps[i].neigh[counter] = j;
							// cout << i << " " << j << endl;
							counter++;
							if(counter==8) break1=true;
							found = true;
							break;
						} 
					}
				}
				if(found) break;
			}
			if(break1) break;
		}
		if (counter==7) Maps[i].neigh[7] = i;
	}
}

game_map::~game_map(){
	
}
