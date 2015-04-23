#include "game_map.h"

game_map::game_map(int n){ //n should be even
	int n2 = n*n;
	for (int i = 0; i < n2; i++){
		Vector3* v = new Vector3[4];
		int* neigh = new int[8];
		v[0].set(-n/2,(i%n)-n/2,i/n-n/2);
		v[1].set(-n/2,(i%n)-n/2,i/n-n/2+1);
		v[2].set(-n/2,(i%n)-n/2-1,i/n-n/2+1);
		v[3].set(-n/2,(i%n)-n/2-1,i/n-n/2);
		if(i==0){
			neigh[0] = 1;
			neigh[1] = n;
			neigh[2] = n+1;
			neigh[3] = n2;
			neigh[4] = 5*n2+n-1;
			neigh[5] = n2+n;
			neigh[6] = 5*n2+2*n-1;
			neigh[7] = -1;
		}
		else if(i==n-1){
			neigh[0] = n-2;
			neigh[1] = 2*n-2;
			neigh[2] = 2*n-1;
			neigh[3] = 2*n2;
			neigh[4] = 2*n2+n;
			neigh[5] = 2*n2-n;
			neigh[6] = 2*n2-2*n;
			neigh[7] = -1;
		}
		else if(i==n2-1){
			neigh[0] = n2-2;
			neigh[1] = n2-n-1;
			neigh[2] = n2-n-2;
			neigh[3] = 3*n2-n;
			neigh[4] = 3*n2-2*n;
			neigh[5] = 3*n2;
			neigh[6] = 3*n2+n;
			neigh[7] = -1;
		} 
		else if(i==n2-n){
			neigh[0] = n2-2*n;
			neigh[1] = n2-2*n+1;
			neigh[2] = n2-n+1;
			neigh[3] = 6*n2-1;
			neigh[4] = 6*n2-n-1;
			neigh[5] = 4*n2-n;
			neigh[6] = 4*n2-2*n;
			neigh[7] = -1;
		} 
		else{
			neigh[0] = i-n-1;
			neigh[1] = i-n;
			neigh[2] = i-n+1;
			neigh[3] = i-1;
			neigh[4] = i+1;
			neigh[5] = i+n-1;
			neigh[6] = i+n;
			neigh[7] = i+n+1;
		}
		grid g(v,neigh);
		Map.push(g);
	}

}

game_map::~game_map(){
	
}