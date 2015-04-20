#include "bots.h"

/*difficulty level (1,2,3)
 *easy->4 level deep
 *medium->8 level deep
 *advanced->12 level deep
*/


bots::bots(int ID,int gr_id,Vector3 pos,int n_lives,int n_kills,int difficulty)
{
	id=ID;
	group_id=gr_id;
	position=pos;
	lives=n_lives;
	last_shot=0.0;
	health=100;
	//kills = 0;
	up=0;
	right=0;
	weaponry=push_back(Weapon_set[0]);
	g_locate=g;
	active=true;
	
	//set mass and radius
	mass=player_mass;
	radius=player_radius;

	difficulty_level=difficulty;
}

//respawn
bool bots::bot_reset()
{
	if(lives==0)
	{	
		for(int i=0;i<gameplay->All_Bots.size();i++)
		{
			if(gameplay->All_Bots[i].id==id)
			{
				gameplay->All_Bots[i].active=false;
				break;
			}	
		}
		for(int i=0;i<gameplay->Map[g_locate].Bots.size();i++)
		{
			if(gameplay->Map[g_locate].Bots[i].id==id)
			{
				gameplay->Map[g_locate].Bots.erase(gameplay->Map[g_locate].Bots.begin()+i);
				break;
			}
		}		
		return false;
	}
	lives-=1;
	health=100;
	last_shot=0.0;
	weaponry.clear();
	weaponry.push_back(Weapon_set[0]);
	up=0;
	down=0;
	return true; 

}

//max_lives and health_increase are global variable
void bots::get_health_pack(int id)
{
	//life pack
	if((id==1)&&(lives<max_lives))
		lives++;
	//health increase
	if(id==2){
		if(health<100-health_increase)
			health+=health_increase;
		else
			health=100;
	}
}

void bots::recovery()
{
	if(last_hit>recovery_time)
		health=(health<(100-health_recovery*DELTA_T))?(health+health_recovery*DELTA_T):100;	
}

void bots::get_weapon(int ID)
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

void bots::toggle_weapon()
{
	if(current_weapon==weaponry.size()-1)
		current_weapon=0;
	else
		current_weapon++;		
}

void bots::fire_weapon()
{
	weaponry[current_weapon].shot_fired();

	//add bullet parameters
	//update grid with bullet
	//update gameplay->Bullets object

	if(weaponry[current_weapon].noshots())
	{
		weaponry.erase(weaponry.begin()+current_weapon);
		current_weapon--;	
	}
}

int bots::Player_CheckCollision()							//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
{
	int index=-1;

	if(gameplay->Map[g_locate].Players.size()>1)
		index=8;
	else 
	{
		for(int i=0;i<8;i++)
		{
			if(gameplay->Map[(neigh[i])].Players.size()>0)
			{
				index=i;
				break;
			}
		}
	}
	return index;	
}

int bots::Bot_CheckCollision()							//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
{
	int index=-1;

	if(gameplay->Map[g_locate].Bots.size()>1)
		index=8;
	else 
	{
		for(int i=0;i<8;i++)
		{
			if(gameplay->Map[(neigh[i])].Bots.size()>0)
			{
				index=i;
				break;
			}
		}
	}
	return index;	
}

int bots::CheckHit()									//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
{
	int index=-1;
	if(gameplay->Map[g_locate].Bullets.size()>0)
		index=8;
	else 
	{
		for(int i=0;i<8;i++)
		{
			if(gameplay->Map[(neigh[i])].Bullets.size()>0)
			{
				index=i;
				break;
			}
		}
	}
	return index;		
}

void bots::Player_SolveCollision()
{
	int i=Player_CheckCollision();
	if(i!=-1)
	{
		int j;
		player p;
		if(i!=8)
		{
			j=(gameplay->All_Players[(gameplay->Map[(neigh[i])].Players[0])].id==id)?1:0;
			p= gameplay->All_Players[(gameplay->Map[(neigh[i])].Players[j])];
		}
		else{
			j=(gameplay->All_Players[(gameplay->Map[g_locate].Players[0])].id==id)?1:0;
			p= gameplay->All_Players[(gameplay->Map[g_locate].Players[j])];
		}
		/*solving collision*/
		Vector3 del_R=(p.position).add(location.neg()).setlen(1);
		Vector3 v1=del_R.setlen(velocity.dot(del_R)),
		v2=del_R.setlen((p.velocity).dot(del_R));

		if((v1.add(v2.neg()).dot(del_R)>0)&&(b2.R).add(b1->R.neg()).mod()<b1->radius+b2.radius)
		{
			Vector3 v1_=velocity.add(v1.neg()),
			v1__=v1.mult((mass-p.mass)/(mass+p.mass)).add(p.velocity.mult((2*p.mass)/(mass+p.mass)));
			velocity=v1__.add(v1_);
		}	
		break;		
		health=(health-collateral_damage<0)?0:(health-collateral_damage);
		
	}
	last_hit=0;
	if(health==0)
		bot_reset();		
}

void bots::Bot_SolveCollision()
{
	int i=Bot_CheckCollision();
	if(i!=-1)
	{
		int j;
		bots p;
		if(i!=8)
		{
			j=(gameplay->All_Bots[(gameplay->Map[(neigh[i])].Bots[0])].id==id)?1:0;
			p= gameplay->All_Bots[(gameplay->Map[(neigh[i])].Bots[j])];
		}
		else{
			j=(gameplay->All_Bots[(gameplay->Map[g_locate].Bots[0])].id==id)?1:0;
			p= gameplay->All_Bots[(gameplay->Map[g_locate].Bots[j])];
		}
		/*solving collision*/
		Vector3 del_R=(p.position).add(location.neg()).setlen(1);
		Vector3 v1=del_R.setlen(velocity.dot(del_R)),
		v2=del_R.setlen((p.velocity).dot(del_R));

		if((v1.add(v2.neg()).dot(del_R)>0)&&(b2.R).add(b1->R.neg()).mod()<b1->radius+b2.radius)
		{
			Vector3 v1_=velocity.add(v1.neg()),
			v1__=v1.mult((mass-p.mass)/(mass+p.mass)).add(p.velocity.mult((2*p.mass)/(mass+p.mass)));
			velocity=v1__.add(v1_);
		}	
		break;		
		health=(health-collateral_damage<0)?0:(health-collateral_damage);
		
	}
	last_hit=0;
	if(health==0)
		bot_reset();		
}

void bots::OnHit()
{
	int i=CheckHit();
	if(i!=-1)
	{
		int j;
		bullet b;
		if(i!=8){
			j=(gameplay->Map[(neigh[i])].Bullets[0].id==id)?1:0;
			b= gameplay->Map[(neigh[i])].Bullets[j];	
		}
		else{
			j=(gameplay->Map[g_locate].Bullets[0].id==id)?1:0;
			b= gameplay->Map[g_locate].Bullets[j];	
		}
		health=(health-b.damage<0)?0:(health-b.damage);
		//remove bullet from grid
		gameplay->Map[(neigh[i])].Bullets.erase(gameplay->Map[(neigh[i])].Bullets.begin()+j);			
		//set last_hit to zero
		last_hit=0;
		if(health==0)
			bot_reset();
	}
}

Vector3 bots::set_velocity(int up,int right,int move_up,int move_right,Vector3 v)
{
	Vector3 final;
	if(move_up!=up)
	{
		Vector3 along=(v.setlen(1.0)).mult((move_up-up)*DELTA_T);
		final=v.add(along);
	}
	if(move_right!=right)
	{
		Vector3 tang=((v.cross(normal)).setlen(1)).mult((move_right-right)*DELTA_T);
		final=v.add(tang);
	}
	if((up==move_up) && (right=move_right))
	{
		if(v.mod()<max_speed)
		{
			Vector3 tang= ((v.cross(normal)).setlen(1)).mult(move_right*DELTA_T);
			Vector3 along= (v.setlen(1)).mult(move_up-*DELTA_T);
			final=v.add(tang.add(along));
		}
		else
			v=v.setlen(max_speed);
	}
	if(move_up=up)
		if(final.mod()<max_speed)
		{
			Vector3 along=(v.setlen(1.0)).mult((move_up-up)*DELTA_T);
			final=v.add(along);
		}		
	if(move_right=right)
	{
		if(final.mod()<max_speed)
		{
			Vector3 tang=(v.setlen(1.0)).mult((move_right-right)*DELTA_T);
			final=v.add(along);
		}
	}
	return final;	
}

void bots::surface_constraint()
{
	float distance= gameplay->Map[g_locate].distance_from_surface(position);
	position=position.add((Map[g_locate].normal).mult(-distance));              
}

int bots::new_location(int move_up,int move_right)
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

void bots::update(vector<key_tap> keys_pressed)
{
	key_tap k;				//for looping over vector<key_taps>
	int move_up=0;			//up movement parameter given by key taps
	int move_right=0;		//right movement parameter given by key taps
	bool changed=false;		//if player position/grid has changed?
	
	//recording keys_pressed and taking corresponding action
	for(int i=0;i<keys_pressed.size();i++)
	{
		k=keys_pressed[i];
		switch(k){
			case rot_left:
				aim=aim.rotate(gameplay->Map[g_locate].normal,rotate*DELTA_T);
				break;
			case rot_right:
				aim=aim.rotate(gameplay->Map[g_locate].normal,-rotate*DELTA_T);
				break;
			case up:
				move_up+=1;					//up and right are class variables
				break;
			case right:
				move_right+=1;
				break;
			case down:
				move_up-=1;
				break;
			case left:
				move_right-=1;
				break;
			case shoot:
				fire_weapon();
				break;
			case toggle:
				toggle_weapon();
				break;				
			default:
				break;
		}
	}
	
	/*Collisions*/
	Player_SolveCollision();
	Bot_SolveCollision();
	OnHit();
	
	/*setting movement parameters*/
	velocity=set_velocity(up,right,move_up,move_right,velocity);
	position=position+velocity*DELTA_T;	
	up=move_up;
	right=move_right;

	/*updating weaponry and health parameters*/
	last_hit+=DELTA_T;
	for(int i=0;i<weaponry.size();i++)
	{
		weaponry[i].last_shot+=DELTA_T;
	}

	/*updating grids*/
	if(new_location(up,right)!=8)
	{
		for(int i=0;i<gameplay->Map[g_locate].Bots.size();i++)
		{
			if(id==gameplay->All_Bots[(gameplay->Map[g_locate].Bots[i])].id)
			{
				gameplay->Map[g_locate].Bots.erase(gameplay->Map[g_locate].Bots.begin()+i);
				break;
			}
		}
		g_locate=neigh[new_location(up,right)];
		surface_constraint();	
		changed=true;
	}

	/*obtain weapon if available*/
	if(gameplay->Map[g_locate].Armoury.size()!=0)
	{
		for(int i=0;i<gameplay->Map[g_locate].Armoury.size();i++)
			weaponry.push_back(Weapon_set(gameplay->Map[g_locate].Armoury[i]));
	}
	/*obtain boost if available*/
	if(gameplay->Map[g_locate].Boosts.size()!=0)
	{
		for(int i=0;i<gameplay->Map[g_locate].Boosts.size();i++)
			get_health_pack(gameplay->Map[g_locate].Boosts[i]);	
	}

	recovery();	
	
	if(changed)
		gameplay->Map[g_locate].Bots.push_back(*this);
	else
	{
		for(int i=0;i<Map[g_locate].Bots.size();i++)
		{
			if(Map[g_locate].Bots[i].id==id)
			{
				Map[g_locate].Bots[i]=*this;
				break;
			}	
		}
	}	
}


direction bots::get_directions(int i)
{
	direction temp;
	switch(i){
		case 0:
			temp.tang=0;
			temp.normal=0;
			break;
		case 1:
			temp.tang=1;
			temp.normal=1;
			break;
		case 2:
			temp.tang=1;
			temp.normal=0;
			break;
		case 3:
			temp.tang=1;
			temp.normal=-1;
			break;
		case 4:
			temp.tang=0;
			temp.normal=-1;
			break;
		case 5:
			temp.tang=-1;
			temp.normal=-1;
			break;
		case 6:
			temp.tang=-1;
			temp.normal=0;
			break;
		case 7:
			temp.tang=-1;
			temp.normal=1;
			break;				
		default:
			temp.tang=0;
			temp.normal=0;
			break;
	}

	return temp;
}


bool bots::present_in_vector(int i,vector<nodes>*checkzones)
{
	for(int j=0;j<checkzones->size();j++)
	{
		if(checkzones[j].id==i)
		{
			return true;
		}
	}
	return false;
}

void bots::add_to_vector(int i,int j,int up,int right,vector<nodes>* checkzones,int level)
{
	//i-> grid id of base grid
	//j-> neighbour index wrt base grid

	if(!present_in_vector(i,checkzones))
	{
		nodes temp;
		temp.id=gameplay->Map[i].neigh[j].id;
		temp.right=right+get_directions(j).normal;
		temp.up=up+get_directions(j).tang;
		checkzones->push_back(temp);
	}
	if(level<4*difficulty_level)
	{
		for(int k=0;k<8;k++){
			int a=up+get_directions(k).tang;
			int b=right+get_directions(k).normal;
			add_to_vector(gameplay->Map[i].neigh[j],k,a,b,checkzones,level+1);
		}
	}
}

void bots::analyze_view()
{
	//clear existing field of view
	field_of_view.view_players.clear();
	field_of_view.view_bots.clear();
	field_of_view.view_bullets.clear();
	field_of_view.view_boosts.clear();
	field_of_view.view_armoury.clear();
	//array to keep track of relative positioning
	int index;
	int move_up=0;
	int move_right=0;
	vector<nodes>* checkzones;
	for(int i=0;i<8;i++)
	{
		add_to_vector(g_locate,i,0,0,checkzones,0);
	}
	for(int i=0;i<checkzones.size();i++)
	{
		int a=checkzones[i].id;
		nodes temp;
		temp.right=checkzones[i].right;
		temp.left=checkzones[i].left;
		for(int j=0;j<gameplay->Map[a].Players.size();j++)
		{
			temp.id=gameplay->Map[a].Players[j];
			field_of_view.view_players.push_back(temp);
		}
		for(int j=0;j<gameplay->Map[a].Bots.size();j++)
		{
			temp.id=gameplay->Map[a].Bots[j];
			field_of_view.view_bots.push_back(temp);
		}
		for(int j=0;j<gameplay->Map[a].Bullets.size();j++)
		{
			temp.id=gameplay->Map[a].Bullets[j];
			field_of_view.view_bullets.push_back(temp);
		}
		for(int j=0;j<gameplay->Map[a].Armoury.size();j++)
		{
			temp.id=gameplay->Map[a].Armoury[j];
			field_of_view.view_armoury.push_back(temp);
		}
		for(int j=0;j<gameplay->Map[a].Boosts.size();j++)
		{
			temp.id=gameplay->Map[a].Boosts[j];
			field_of_view.view_boosts.push_back(temp);
		}
	}		
}

vector<int> bots::projected_path(int i,int j,int difficulty)			//i is the player id,j=0 for player,1 for bot,2 for bullet
{
	vector<int> trajectory;
	if(j==0)
		{player p=gameplay->All_Players[i];}	
	else if(j==1)
		{bots p=gameplay->All_Bots[i];}
	if(p.up==0 && p.right==0)
	{
		for(int k=0;k<difficulty;k++)
			trajectory.push_back(g_locate);
		return trajectory;
	}	
	int a = gameplay->Map[p.g_locate]
	int b = new_location(p.up,p.right);
	for(int k=0;k<difficulty;k++)
	{
		trajectory.push_back(a.neigh[b]);
		a=a.neigh[b];
	}	
	return trajectory;

}	

void bots::projected_collision()
{
	player_collision.clear();
	bots_collision.clear();

	vector<int> path=projected_path(id,1,difficulty_level);
	vector<int> opponent_path;
	for(int i=0;i<field_of_view.view_players.size();i++)
	{
		int collide=-1;
		opponent_path = projected_path(field_of_view.view_players[i].up,field_of_view.view_players[i].right,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(path[j]==opponent_path[j])
			{
				collide=j;
				break;
			}	
		}
		player_collision.push_back(collide);	
	}

	for(int i=0;i<field_of_view.view_bots.size();i++)
	{
		int collide=-1;
		opponent_path = projected_path(field_of_view.view_bots[i].up,field_of_view.view_bots[i].right,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(path[j]==opponent_path[j])
			{
				collide=j;
				break;
			}	
		}
		bots_collision.push_back(collide);	
	}
}

vector<int> bot::level_1_projected_path(int i,int j,int difficulty)
{
	vector<int> path = projected_path(i,j,difficulty);
	vector<int> level_1_path;
	for(int k=1;k<path.size();k++)
		level_1_path.push_back(path[k]);
	int last= path.back();
	if(j==0)
		{player p=gameplay->All_Players[i];}	
	else if(j==1)
		{bots p=gameplay->All_Bots[i];}
	if(p.up==0 && p.right==0)
	{
		level_1_path.push_back[(path[0])];
		return path;
	}
	else{
		int a = gameplay->Map[path.back()].neigh(new_location(p.up,p.right));
		level_1_path.push_back[a];
		return path;	
	}
}

void bots::level_1_projected_collision(int move_up,int move_right)
{
	level_1_player_collision.clear();
	level_1_bots_collision.clear();

	//save present movement directions
	int a=up;
	int b=right;
	int c=g_locate;

	//change movement directions to input parameters
	up=move_up;
	right=move_right;

	vector<int> path=level_1_projected_path(id,1,difficulty_level);

	vector<int> opponent_path;
	for(int i=0;i<field_of_view.view_players.size();i++)
	{
		int collide=-1;
		opponent_path = level_1_projected_path(field_of_view.view_players[i].up,field_of_view.view_players[i].right,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(path[j]==opponent_path[j])
			{
				collide=j;
				break;
			}	
		}
		level_1_player_collision.push_back(collide);	
	}

	for(int i=0;i<field_of_view.view_bots.size();i++)
	{
		int collide=-1;
		opponent_path = level_1_projected_path(field_of_view.view_bots[i].up,field_of_view.view_bots[i].right,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(path[j]==opponent_path[j])
			{
				collide=j;
				break;
			}	
		}
		level_1_bots_collision.push_back(collide);	
	}
}





