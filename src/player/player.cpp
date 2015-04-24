#include "player.h"
#include <iostream>
//gameplay is the game_Maps object

player::player()
{}

player::player(int ID,int gr_id,Vector3 pos,int n_lives,int n_kills,int g)
{
	id=ID;
	group_id=gr_id;
	position=pos;
	lives=n_lives;
	last_hit=0;
	health=100;
	fwd=0;
	tang=0;
	weaponry.push_back(Weapon_set[0]);
	g_locate=g;
	active=true;
	current_weapon=0;
	//set mass and radius
	mass=player_mass;
	radius=player_radius;
}

// player::player(player &obj)
// {
// 	name = *obj.name;
// 	id = *obj.id;
// 	group_id = *obj.group_id;
// 	g_locate = *obj.g_locate;
// 	fwd = *obj.fwd;
// 	tang = *obj.tang;
// 	mass = *obj.mass;
// 	radius = *obj.radius;
// 	active = *obj.active;
// 	health = *obj.health;
// 	weaponry = *obj.weaponry;
// 	position = *obj.position;
// 	velocity = *obj.velocity;
// 	lives = *obj.lives;
// 	last_hit = *obj.last_hit;
// 	current_weapon = *obj.current_weapon;
// }

//respawn
void player::player_reset()
{
	if(lives==0)
	{	
		(gameplay->All_Players[id]).active=false;
		
		for(int i=0;i<gameplay->Maps[g_locate].Players.size();i++)
		{
			if(gameplay->Maps[g_locate].Players[i] == id)
			{
				gameplay->Maps[g_locate].Players.erase(gameplay->Maps[g_locate].Players.begin()+i);
				break;
			}
		}
		return;		
	}
	lives-=1;
	health=100;
	last_hit=0;
	weaponry.clear();
	weaponry.push_back(Weapon_set[0]);
	fwd=0;
	tang=0; 
}

//max_lives and health_increase are global variables
void player::get_health_pack(int id)
{
	//life pack
	if((id==1)&&(lives<max_lives))
		lives++;

	//health increase
	if(id==2){
		if(health<100-25)
			health+=25;
		else
			health=100;
	}
}

void player::recovery()
{
	if(last_hit>recovery_time)
		health=(health<(100-health_recovery*DELTA_T))?(health+health_recovery*DELTA_T):100;	
}

void player::get_weapon(int ID)
{	
	//ID -> corresponding to Weapon_set

	int present=-1;
	//check if weapon is present in weaponry
	for(int i=0;i<weaponry.size();i++)
	{
		if(ID==(weaponry[i]).ID)
		{
			present=i;
			break;
		}
	}
	if(present!=-1)
		(weaponry[present]).upgrade_weapon();
	else
		weaponry.push_back(Weapon_set[ID]);
		current_weapon++;	
}

void player::toggle_weapon()
{
	if(current_weapon==weaponry.size()-1)
		current_weapon=0;
	else
		current_weapon++;		
}

void player::fire_weapon(int i)
{
	// cout<<"command issued"<< "true" <<endl;
	weaponry[current_weapon].shot_fired();
	// cout<< "shot fired" <<endl;
	Vector3 d;
	Vector3 normal = gameplay->Maps[g_locate].normal;
	// cout<<"normal set"<<endl;
	switch(i)
	{
		case 0:
			d=velocity.setlen(1);
			break;
		case 1:
			d=(velocity.cross(normal)).setlen(1);
			break;
		case 2:
			d=(velocity.setlen(1)).neg();
			break;
		case 3:
			d=((velocity.cross(normal)).setlen(1)).neg();
			break;	
	}
	// cout<<"direction set"<<endl;
	// cout<<"weapon size"<< weaponry.size()<<endl;
	// cout<<"current weapon"<<current_weapon<<endl;
	bullet* b=new bullet(id,group_id,weaponry[current_weapon].ID,position,d,g_locate,gameplay->Maps[g_locate].normal);
	// cout << "player weapon id " << weaponry[current_weapon].ID << endl;
	//  cout<<"timespan1" << Weapon_set[0].timespan<<endl;
	//  cout<<"timespan2" << Weapon_set[b->weapon_ID].timespan<<endl;
	gameplay->Bullets.push_back(*b);
	(gameplay->Maps[g_locate].Bullets).push_back(*b);
	// cout<<"grid set"<<endl;
	if(weaponry[current_weapon].no_shots())
	{
		weaponry.erase(weaponry.begin()+current_weapon);
		current_weapon--;	
	}
}

int player::Player_CheckCollision()							//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
{
	int index=-1;

	if(gameplay->Maps[g_locate].Players.size()>1)
		index=8;
	else 
	{
		for(int i=0;i<8;i++)
		{
			int neighbour = gameplay->Maps[g_locate].neigh[i];
			if(gameplay->Maps[(neighbour)].Players.size()>0)
			{
				index=i;
				break;
			}
		}
	}
	return index;	
}

int player::Bot_CheckCollision()							//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
{
	int index=-1;

	if(gameplay->Maps[g_locate].Bots.size()>0)
		index=8;
	else 
	{
		for(int i=0;i<8;i++)
		{
			int neighbour = gameplay->Maps[g_locate].neigh[i];
			if(gameplay->Maps[neighbour].Bots.size()>0)
			{
				index=i;
				break;
			}
		}
	}
	return index;	
}

// int player::CheckHit()									//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
// {
// 	int index=-1;
// 	for(int i=0;i<gameplay->Maps[g_locate].Bullets.size();i++)
// 	{
// 		if(gameplay->Maps[g_locate].Bullets[i].player_ID!=id)
// 			return 8;	
// 	}	
		
	
// 	else 
// 	{
// 		for(int i=0;i<8;i++)
// 		{
// 			int neighbour = gameplay->Maps[g_locate].neigh[i];
// 			for(int j=0;j<gameplay->Maps[g_locate].Bullets.size();j++)
// 			{
// 				if(gameplay->Maps[g_locate].Bullets[i].player_ID!=id)
// 				{

// 				}	
// 			}
// 		}
// 	}
// 	return index;		
// }

void player::Player_SolveCollision()
{
	int i=Player_CheckCollision();
	int j;
	player* p = new player();
	if(i!=-1)
	{
		if(i!=8)
		{
			int neighbour = gameplay->Maps[g_locate].neigh[i]; 
			*p= gameplay->All_Players[((gameplay->Maps[neighbour]).Players[j])];
		}
		else{
			j=(gameplay->Maps[g_locate].Players[0]==id)?1:0;
			*p= gameplay->All_Players[((gameplay->Maps[g_locate]).Players[j])];
		}
		/*solving collision*/
		Vector3 del_R=(p->position).add(position.neg()).setlen(1);
		Vector3 v1=del_R.setlen(velocity.dot(del_R)),
		v2=del_R.setlen((p->velocity).dot(del_R));

		if((v1.add(v2.neg()).dot(del_R)>0)&&(p->position).add(position.neg()).mod()<radius+p->radius)
		{
			Vector3 v1_=velocity.add(v1.neg()),
			v1__=v1.mult((mass-p->mass)/(mass+p->mass)).add(p->velocity.mult((2*p->mass)/(mass+p->mass)));
			velocity=v1__.add(v1_);
		}			
		health=(health-collateral_damage<0)?0:(health-collateral_damage);
		
	}
	last_hit=0;
	if(health==0)
		player_reset();		
}

void player::Bot_SolveCollision()
{
	int i=Bot_CheckCollision();
	int j;
	bots* p = new bots();

	if(i!=-1)
	{

		if(i!=8)
		{
			int neighbour = gameplay->Maps[g_locate].neigh[i]; 
			*p= gameplay->All_Bots[(gameplay->Maps[neighbour].Bots[j])];
		}
		else{
			*p= gameplay->All_Bots[(gameplay->Maps[g_locate].Bots[j])];
		}
		/*solving collision*/
		Vector3 del_R=(p->position).add(position.neg()).setlen(1);
		Vector3 v1=del_R.setlen(velocity.dot(del_R)),
		v2=del_R.setlen((p->velocity).dot(del_R));

		if((v1.add(v2.neg()).dot(del_R)>0)&&((p->position).add(position.neg()).mod()<radius+p->radius))
		{
			Vector3 v1_=velocity.add(v1.neg()),
			v1__=v1.mult((mass-p->mass)/(mass+p->mass)).add(p->velocity.mult((2*p->mass)/(mass+p->mass)));
			velocity=v1__.add(v1_);
		}		
		health=(health-collateral_damage<0)?0:(health-collateral_damage);
	}
	last_hit=0;
	if(health==0)
		player_reset();		
}

void player::OnHit()
{

	int grid_index=-1;
	int bullet_index = -1;
	for(int i=0;i<gameplay->Maps[g_locate].Bullets.size();i++)
	{
		if(gameplay->Maps[g_locate].Bullets[i].player_ID!=id)
		{
			grid_index = 8;
			bullet_index=i;		
			break;
		}
	}	
	if(grid_index!=8) 
	{
		for(int i=0;i<8;i++)
		{
			int neighbour = gameplay->Maps[g_locate].neigh[i];
			for(int j=0;j<gameplay->Maps[neighbour].Bullets.size();j++)
			{
				if(gameplay->Maps[neighbour].Bullets[j].player_ID!=id)
				{
					bullet_index = j;
					grid_index = i;
					break;
				}	
			}
		}
	}

	bullet* b = new bullet();
	int neighbour;
	if(grid_index!=-1)
	{

		if(grid_index!=8){
			neighbour = (gameplay->Maps[g_locate]).neigh[grid_index];
			*b= gameplay->Maps[neighbour].Bullets[bullet_index];	
		}
		else{
			neighbour = g_locate;
			*b= gameplay->Maps[g_locate].Bullets[bullet_index];	
		}
		health=(health-b->damage<0)?0:(health-b->damage);
		
		//remove bullet from grid and game Maps
		gameplay->Maps[neighbour].Bullets.erase(gameplay->Maps[neighbour].Bullets.begin()+bullet_index);			
		
		for(int k=0;k<gameplay->Bullets.size();k++)
		{
			if(gameplay->Bullets[k].bullet_id == b->bullet_id)
			{
				gameplay->Bullets.erase(gameplay->Bullets.begin()+k);
				break;
			}			
		}

		//set last_hit to zero
		last_hit=0;
		if(health==0)
			player_reset();
	}
}

Vector3 player::set_velocity(int up,int right,int move_up,int move_right,Vector3 v)
{
	Vector3 final;
	Vector3 normal = gameplay->Maps[g_locate].normal;
	// cout << "normal " << normal.x << " " << normal.y << " " << normal.z << endl; 
	
	if(v.mod()==0)
	{
		Vector3 index= gameplay->Maps[g_locate].normal;
		if((index.x == 0 && index.y==0) || (index.x==0 && index.z==0))  
			v.set(0.000001,0.0,0.0);
		else 
			v.set(0.0,0.000001,0.0);
	}
	final=v;
	Vector3 changeAlong(v.setlen(0.1));
	Vector3 changePerp((v.cross(normal.neg())).setlen(0.05));
	// cout << "changePerp " << changePerp.x << " " << changePerp.y << " " << changePerp.z << endl;
	if(move_up==-1)
		final = final.mult(0.8);
	else if(move_up==1)
		final = final.add(changeAlong);

	if(move_right==-1)
		final = final.add(changePerp);
	else if(move_right==1)
		final = final.sub(changePerp);

	if (move_up==0 && move_right==0)
		final = final.mult(0.95);

	if(final.mod()>max_speed)
		final = final.setlen(max_speed);

	// if(move_up!=up)
	// {
	// 	Vector3 along=(v.setlen(1.0)).mult((move_up-up)*DELTA_T);
	// 	final=final.add(along);
	// }
	// if(move_right!=right)
	// {
	// 	Vector3 ta=((v.cross(normal)).setlen(1)).mult((move_right-right)*DELTA_T);
	// 	final=final.add(ta);
	// }
	// if((up==move_up) && (right==move_right))
	// {
	// 	if(final.mod()<max_speed)
	// 	{
	// 		float factor = move_up*(-DELTA_T);
	// 		Vector3 ta= ((v.cross(normal)).setlen(1)).mult(factor);
	// 		Vector3 along= (v.setlen(1)).mult(factor);
	// 		final=final.add(ta.add(along));
	// 	}
	// 	else
	// 		final=final.setlen(max_speed);
	// }
	// if(move_up==up)
	// 	if(final.mod()<max_speed)
	// 	{
	// 		Vector3 along=(v.setlen(1.0)).mult((move_up-up)*DELTA_T);
	// 		final=final.add(along);
	// 	}		
	// if(move_right==right)
	// {
	// 	if(final.mod()<max_speed)
	// 	{
	// 		Vector3 ta=(v.setlen(1.0)).mult((move_right-right)*DELTA_T);
	// 		final=final.add(ta);
	// 	}
	// }
	return final;
}

void player::surface_constraint()
{
	float distance= gameplay->Maps[g_locate].distance_from_surface(position);
	position=position.add(((gameplay->Maps[g_locate]).normal).mult(-distance));
	bool a = gameplay->Maps[g_locate].point_in_plane(position);              
	// cout<< "surface " << a <<endl;


}

int player::new_location(int move_up,int move_right)
{
	if(move_up==0 && move_right==1)
		return 0;
	else if(move_up==1 && move_right==1)
		return 1;
	else if(move_up==1 && move_right==0)
		return 2;
	else if(move_up==1 && move_right==-1)
		return 3;
	else if(move_up==0 && move_right==-1)
		return 4;
	else if(move_up==-1 && move_right==-1)
		return 5;
	else if(move_up==-1 && move_right==0)
		return 6;
	else if(move_up==-1 && move_right==1)
		return 7;
	else if(move_up==0 && move_right==0)
		return 8;
}

void player::update(vector<int> keys_pressed)
{
	// key_tap k;				//for looping over vector<key_taps>
	int k;
	int move_up=0;			//up movement parameter given by key taps
	int move_right=0;		//right movement parameter given by key taps
	bool changed=false;		//if player position/grid has changed?
	bool shoot=false;

	//recording keys_pressed and taking corresponding action
	for(int i=0;i<keys_pressed.size();i++)
	{
		k=keys_pressed[i];
		switch(k){
			case 0:
				move_up+=1;					//up and right are class variables
				break;
			case 1:
				move_right+=1;
				break;
			case 2:
				move_up-=1;
				break;
			case 3:
				move_right-=1;
				break;
			case 4:
				if(!shoot)
				{
					fire_weapon(0);
					shoot=true;
					break;
				}
			case 5:
				if(!shoot)
				{
					fire_weapon(1);
					shoot=true;
					break;
				}
			case 6:
				if(!shoot)
				{
					fire_weapon(2);
					shoot=true;
					break;
				}
			case 7:
				if(!shoot)
				{
					fire_weapon(3);
					shoot=true;
					break;
				}	
			case 8:
				toggle_weapon();
				break;				
			default:
				break;
		}
	}
	
	/*Collisions*/
	// Player_SolveCollision();
	// Bot_SolveCollision();
	// OnHit();
	
	/*setting movement parameters*/
	velocity=set_velocity(fwd,tang,move_up,move_right,velocity);
	// cout<< "velocity x " <<velocity.x << "velocity y "<< velocity.y << "velocity z "<< velocity.z<<endl;
	position=position.add(velocity.mult(DELTA_T));	
	// cout<< "position x " <<position.x << "position y "<< position.y << "position z "<< position.z<<endl;
	fwd=move_up;
	tang=move_right;
	// cout<< "fwd "<< fwd<<endl;
	// cout<< "tang "<< tang<<endl;

	/*updating weaponry and health parameters*/
	last_hit+=1;
	for(int i=0;i<weaponry.size();i++)
	{
		weaponry[i].last_shot+=1;
	}

	/*if(new_location(up,right)!=8)
	{
		for(int i=0;i<gameplay->Maps[g_locate].Players.size();i++)
		{
			if(id==gameplay->All_Players[(gameplay->Maps[g_locate].Players[i])].id)
			{
				gameplay->Maps[g_locate].Players.erase(gameplay->Maps[g_locate].Players.begin()+i);
				break;
			}
		}
		g_locate=neigh[new_location(up,right)];
		surface_constraint();	
		changed=true;
	}*/


	/*updating grids*/

	int index= gameplay->Maps[g_locate].find_grid(position);
	if(index<8)
	{
		/*remove player from present grid*/
		for(int i=0;i<gameplay->Maps[g_locate].Players.size();i++)
		{
			if(id==(gameplay->Maps[g_locate].Players[i]))
			{
				gameplay->Maps[g_locate].Players.erase(gameplay->Maps[g_locate].Players.begin()+i);
				break;
			}
		}

		changed=true;
		// cout<<"changed "<<changed<<endl;
		Vector3 oldNormal = gameplay->Maps[g_locate].normal;
		g_locate=gameplay->Maps[g_locate].neigh[index];
		// Vector3 align = gameplay->Maps[g_locate].normal;
		// Vector3 align2 = gameplay->Maps[g_locate].normal;
		// float rad = (align.dot(align2))/((align.mod())*align2.mod());
		// velocity = velocity.rotate(align2.cross(align),rad);
		Vector3 newNormal= gameplay->Maps[g_locate].normal;
		if (!newNormal.equal(oldNormal)){
			if(newNormal.x==0 && newNormal.y==0){
				if(velocity.x==0){
					velocity.x = velocity.z;
					velocity.z = 0;
					if(velocity.dot(oldNormal)>0)
						velocity.x = -velocity.x;
				}
				else if(velocity.y==0){
					velocity.y = velocity.z;
					velocity.z = 0;
					if(velocity.dot(oldNormal)>0)
						velocity.y = -velocity.y;
				}
			}
			else if(newNormal.y==0 && newNormal.z==0){
				if(velocity.y==0){
					velocity.y = velocity.x;
					velocity.x = 0;
					if(velocity.dot(oldNormal)>0)
						velocity.y = -velocity.y;
				}
				else if(velocity.z==0){
					velocity.z = velocity.x;
					velocity.x = 0;
					if(velocity.dot(oldNormal)>0)
						velocity.z = -velocity.z;
				}
			}
			else if(newNormal.z==0 && newNormal.x==0){
				if(velocity.z==0){
					velocity.z = velocity.y;
					velocity.y = 0;
					if(velocity.dot(oldNormal)>0)
						velocity.z = -velocity.z;
				}
				else if(velocity.x==0){
					velocity.x = velocity.y;
					velocity.y = 0;
					if(velocity.dot(oldNormal)>0)
						velocity.x = -velocity.x;
				}
			}
		}
		//add player to new grid
		gameplay->Maps[g_locate].Players.push_back(id);
		
		surface_constraint();
	}		

	else if (index==9){
		cout << "9 Assmpn not true" << endl;
	}
	/*obtain weapon if available*/
	if(gameplay->Maps[g_locate].Armoury.size()!=0)
	{
		for(int i=0;i<gameplay->Maps[g_locate].Armoury.size();i++)
			get_weapon(gameplay->Maps[g_locate].Armoury[i]);
	}
	
	/*obtain boost if available*/
	if(gameplay->Maps[g_locate].Boosts.size()!=0)
	{
		for(int i=0;i<gameplay->Maps[g_locate].Boosts.size();i++)
			get_health_pack(gameplay->Maps[g_locate].Boosts[i]);	
	}

	/*health recovery*/
	recovery();

	/*update weapons*/
	for(int i=0;i<weaponry.size();i++)
	{
		weaponry[i].last_shot+=1;
	}	

}

player::~player(){
	
}
