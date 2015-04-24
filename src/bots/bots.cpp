#include "bots.h"

/*difficulty level (1,2,3)
 *easy->4 level deep
 *medium->8 level deep
 *advanced->1ut2  deep
*/
bots::bots()
{}

bots::~bots()
{}

bots::bots(int ID,int gr_id,Vector3 pos,int n_lives,int difficulty,int g)
{
	id=ID;
	group_id=gr_id;
	position=pos;
	lives=n_lives;
	last_hit=0;
	health=100;
	//kills = 0;
	fwd=0;
	tang=0;
	weaponry.push_back(Weapon_set[0]);
	g_locate=g;
	active=true;
	current_weapon=0;
	//set mass and radius
	mass=player_mass;
	radius=player_radius;

	difficulty_level=difficulty;
	field_of_view = new field_view();

	while(state_stack.size()!=0)
	{
		state_stack.pop();
	}	
	state_stack.push("wander");
}

bots::bots(const bots& b)
{
	id= b.id;
	group_id= b.group_id;
	g_locate= b.g_locate;
	fwd= b.fwd;
	tang= b.tang;
	mass= b.mass;
	radius = b.radius;
	position = b.position;
	velocity = b.velocity;
	active=b.active;
	health= b.health;
	field_of_view= new field_view();
	field_of_view=b.field_of_view;
	last_hit= b.last_hit;
	difficulty_level = b.difficulty_level;
	weaponry = b.weaponry;
	lives = b.lives;
	current_weapon = b.current_weapon;
	checkzones = b.checkzones;
	state_stack = b.state_stack;
	player_collision= b.player_collision;
	bots_collision= b.bots_collision;
	level_1_player_collision = b.level_1_player_collision;
	level_1_bots_collision= b.level_1_bots_collision;
	level_1_bullet_collision = b.level_1_bullet_collision;
	ans= b.ans;
}

//respawn
void bots::bot_reset()
{
	if(lives==0)
	{	
		gameplay->All_Bots[id].active=false;
		
		for(int i=0;i<gameplay->Maps[g_locate].Bots.size();i++)
		{
			if(gameplay->Maps[g_locate].Bots[i] == id)
			{
				gameplay->Maps[g_locate].Bots.erase(gameplay->Maps[g_locate].Bots.begin()+i);
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
	state_stack.push("wander");
}

//max_lives and health_increase are global variable
void bots::get_health_pack(int id)
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

void bots::recovery()
{
	if(last_hit>recovery_time)
		health=(health<(100-health_recovery))?(health+health_recovery):100;	
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

void bots::fire_weapon(int i)
{
	weaponry[current_weapon].shot_fired();
	Vector3 d;
	Vector3 normal = gameplay->Maps[g_locate].normal;
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
	bullet b(-id-1,group_id,weaponry[current_weapon].ID,position,d,g_locate,gameplay->Maps[g_locate].normal);
	gameplay->Bullets.push_back(b);
	(gameplay->Maps[g_locate].Bullets).push_back(b);

	if(weaponry[current_weapon].no_shots())
	{
		weaponry.erase(weaponry.begin()+current_weapon);
		current_weapon--;	
	}
}

int bots::Player_CheckCollision()							//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
{
	int index=-1;

	if(gameplay->Maps[g_locate].Players.size()>0)
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

int bots::Bots_CheckCollision()							//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
{
	int index=-1;

	if(gameplay->Maps[g_locate].Bots.size()>1)
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

// int bots::CheckHit()									//-1 if no collision,0 if collision in grid itself,(1,2,3,4)-collision in neighbour cells
// {
// 	int index=-1;
// 	if(gameplay->Maps[g_locate].Bullets.size()>0)
// 		index=8;
// 	else 
// 	{
// 		for(int i=0;i<8;i++)
// 		{
// 			int neighbour = gameplay->Maps[g_locate].neigh[i];
// 			if(gameplay->Maps[neighbour].Bullets.size()>0)
// 			{
// 				index=i;
// 				break;
// 			}
// 		}
// 	}
// 	return index;		
// }

void bots::Player_SolveCollision()
{
	int i=Player_CheckCollision();
	cout << i << endl;
	int j;
	player* p = new player();
	if(i!=-1)
	{
		if(i!=8)
		{
			int neighbour = gameplay->Maps[g_locate].neigh[i]; 
			*p= gameplay->All_Players[((gameplay->Maps[neighbour]).Players[0])];
		}
		else{
			*p= gameplay->All_Players[((gameplay->Maps[g_locate]).Players[0])];
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
		bot_reset();		
}

void bots::Bots_SolveCollision()
{
	int i=Bots_CheckCollision();
	int j;
	bots p;

	if(i!=-1)
	{

		if(i!=8)
		{
			int neighbour = gameplay->Maps[g_locate].neigh[i]; 
			// j=(gameplay->Maps[neighbour].Bots[0]==id)?1:0;
			p= gameplay->All_Bots[(gameplay->Maps[neighbour].Bots[0])];
		}
		else{
			j=(gameplay->Maps[g_locate].Bots[0]==id)?1:0;
			p= gameplay->All_Bots[(gameplay->Maps[g_locate].Bots[j])];
		}
		/*solving collision*/
		Vector3 del_R=(p.position).add(position.neg()).setlen(1);
		Vector3 v1=del_R.setlen(velocity.dot(del_R)),
		v2=del_R.setlen((p.velocity).dot(del_R));

		if((v1.add(v2.neg()).dot(del_R)>0)&&((p.position).add(position.neg()).mod()<radius+p.radius))
		{
			Vector3 v1_=velocity.add(v1.neg()),
			v1__=v1.mult((mass-p.mass)/(mass+p.mass)).add(p.velocity.mult((2*p.mass)/(mass+p.mass)));
			velocity=v1__.add(v1_);
		}		
		health=(health-collateral_damage<0)?0:(health-collateral_damage);
	}
	last_hit=0;

	if(health==0)
		bot_reset();		
}
void bots::OnHit()
{
	// cout<<"on hit"<<endl;
	int grid_index=-1;
	int bullet_index = -1;
	//cout<<
	for(int i=0;i<gameplay->Maps[g_locate].Bullets.size();i++)
	{
		if(gameplay->Maps[g_locate].Bullets[i].player_ID!=-id-1)
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
				if(gameplay->Maps[neighbour].Bullets[j].player_ID!=-id)
				{
					bullet_index = j;
					grid_index = i;
					break;
				}	
			}
		}
	}

	bullet b;
	int neighbour;
	cout<<"grid_index "<<grid_index<<endl;
	if(grid_index!=-1)
	{
		cout<<"on hit"<<endl;
		if(grid_index!=8){
			neighbour = (gameplay->Maps[g_locate]).neigh[grid_index];
			b= gameplay->Maps[neighbour].Bullets[bullet_index];	
		}
		else{
			neighbour = g_locate;
			b= gameplay->Maps[g_locate].Bullets[bullet_index];	
		}
		health=(health-b.damage<0)?0:(health-b.damage);
		
		//remove bullet from grid and game Maps
		gameplay->Maps[neighbour].Bullets.erase(gameplay->Maps[neighbour].Bullets.begin()+bullet_index);			
		
		for(int k=0;k<gameplay->Bullets.size();k++)
		{
			if(gameplay->Bullets[k].bullet_id == b.bullet_id)
			{
				gameplay->Bullets.erase(gameplay->Bullets.begin()+k);
				break;
			}			
		}

		//set last_hit to zero
		last_hit=0;
		if(health==0)
			bot_reset();
	}
}



Vector3 bots::set_velocity(int up,int right,int move_up,int move_right,Vector3 v)
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
	Vector3 changePerp((v.cross(normal)).setlen(0.05));
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

void bots::surface_constraint()
{
		float distance= gameplay->Maps[g_locate].distance_from_surface(position);
		cout << "distance " << distance << endl;
	position=position.add(((gameplay->Maps[g_locate]).normal).mult(-distance));
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

void bots::update()
{
	// cout<<"starting update"<<endl;
	stack_change();	
	int k;				//for looping over vector<key_taps>
	int move_up=0;			//up movement parameter given by key taps
	int move_right=0;		//right movement parameter given by key taps
	bool changed=false;		//if player position/grid has changed?
	bool shoot=false;

	for(int i=0;i<ans.size();i++)
	{
		// cout<< "key pressed "<<ans[i]<<endl;
	}

	//recording keys_pressed and taking corresponding action
	for(int i=0;i<ans.size();i++)
	{
		k=ans[i];
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
					fire_weapon(1);
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
	cout << "sadas1" << endl;	// cout<<"key reads done"<<endl;

	/*Collisions*/
	Player_SolveCollision();
	Bots_SolveCollision();
	cout << "sadas2" << endl;

	OnHit();
	// cout<<"collisions checked"<<endl;

	/*setting movement parameters*/
	velocity=set_velocity(fwd,tang,move_up,move_right,velocity);
	position=position.add(velocity.mult(DELTA_T));	
	fwd=move_up;
	tang=move_right;

	// cout<<"motion set"<<endl;

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

	// cout<<"weaponry and health set"<<endl;	

	/*updating grids*/
	int index= gameplay->Maps[g_locate].find_grid(position);
	if(index<8)
	{
		/*remove player from present grid*/
		for(int i=0;i<gameplay->Maps[g_locate].Bots.size();i++)
		{
			if(id==(gameplay->Maps[g_locate].Bots[i]))
			{
				gameplay->Maps[g_locate].Bots.erase(gameplay->Maps[g_locate].Bots.begin()+i);
				break;
			}
		}

		//changed=true;
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
		//add bot to new grid
		gameplay->Maps[g_locate].Bots.push_back(id);
		
		surface_constraint();
	}		

	else if (index==9){
		cout << "9 Assmpn not true" << endl;
	}		
	// cout<<"grids updated"<<endl;

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

	// cout<<"recovery done"<<endl;
	/*update weapons*/
	for(int i=0;i<weaponry.size();i++)
	{
		weaponry[i].last_shot+=1;
	}
}

direction* bots::get_directions(int i)
{
	direction* temp = new direction();
	switch(i){
		case 0:
			temp->tang=0;
			temp->normal=0;
			break;
		case 1:
			temp->tang=1;
			temp->normal=1;
			break;
		case 2:
			temp->tang=1;
			temp->normal=0;
			break;
		case 3:
			temp->tang=1;
			temp->normal=-1;
			break;
		case 4:
			temp->tang=0;
			temp->normal=-1;
			break;
		case 5:
			temp->tang=-1;
			temp->normal=-1;
			break;
		case 6:
			temp->tang=-1;
			temp->normal=0;
			break;
		case 7:
			temp->tang=-1;
			temp->normal=1;
			break;				
		default:
			temp->tang=0;
			temp->normal=0;
			break;
	}

	return temp;
}


bool bots::present_in_vector(int i)
{
	for(int j=0;j<checkzones.size();j++)
	{
		if(checkzones[j].id==i)
		{
			return true;
		}
	}
	return false;
}

void bots::add_to_vector(int i,int j,int up,int right,int level)
{
	//i-> grid id of base grid
	//j-> neighbour index wrt base grid

	if(!present_in_vector(i))
	{
		nodes temp;
		
		temp.id=gameplay->Maps[i].neigh[j];
		temp.right=right+get_directions(j)->normal;
		temp.up=up+get_directions(j)->tang;
		checkzones.push_back(temp);
	}
	if(level<difficulty_level-1)
	{
		for(int k=0;k<8;k++){
			int a=up+get_directions(k)->tang;
			int b=right+get_directions(k)->normal;
			add_to_vector(gameplay->Maps[i].neigh[j],k,a,b,level+1);
		}
	}
}

void bots::analyze_view()
{
	

	//clear existing field of view
	field_of_view->view_players.clear();
	field_of_view->view_bots.clear();
	field_of_view->view_bullets.clear();
	field_of_view->view_boosts.clear();
	field_of_view->view_armoury.clear();

	//array to keep track of relative positioning
	// cout<<"cleared fields"<<endl;	

	int index;
	int move_up=0;
	int move_right=0;
	checkzones.clear();
	for(int i=0;i<8;i++)
	{
		// cout<<"to add"<<endl;
		add_to_vector(g_locate,i,0,0,0);
	}
	// cout<<"added to vector"<<endl;
	cout<<"checkzones : "<<checkzones.size()<<endl;
	for(int i=0;i<checkzones.size();i++)
	{
		int a=checkzones[i].id;
		nodes temp;
		temp.right=checkzones[i].right;
		temp.up=checkzones[i].up;
		cout<<"hello  "<<a<<endl;

		for(int j=0;j<gameplay->Maps[a].Players.size();j++)
		{
			temp.id=gameplay->Maps[a].Players[j];
			field_of_view->view_players.push_back(temp);
		}
		for(int j=0;j<gameplay->Maps[a].Bots.size();j++)
		{
			temp.id=gameplay->Maps[a].Bots[j];
			if(temp.id != g_locate)
				field_of_view->view_bots.push_back(temp);
		}
		for(int j=0;j<gameplay->Maps[a].Bullets.size();j++)
		{
			bullet b=gameplay->Maps[a].Bullets[j];
			field_of_view->view_bullets.push_back(b);
		}
		for(int j=0;j<gameplay->Maps[a].Armoury.size();j++)
		{
			temp.id=gameplay->Maps[a].Armoury[j];
			field_of_view->view_armoury.push_back(temp);
		}
		for(int j=0;j<gameplay->Maps[a].Boosts.size();j++)
		{
			temp.id=gameplay->Maps[a].Boosts[j];
			field_of_view->view_boosts.push_back(temp);
		}
	}		
}

vector<int> bots::projected_path(int i,int j,int difficulty)			//i is the player id,j=0 for player,1 for bot,2 for bullet
{
	vector<int> trajectory;
	// player* p = new player();
	// bots* b  = new bots();
	if(j==0)
	{	
		cout<<"player entere"<<endl;
		player p=gameplay->All_Players[i];
		cout<<"grid id"<<p.g_locate<<endl;
		if(p.velocity.mod()==0)
		{	
			
			for(int k=0;k<difficulty;k++)
				trajectory.push_back(p.g_locate);
			return trajectory;
		}	
		grid a = gameplay->Maps[p.g_locate];
		//int b = new_location(p.up,p.right);
		int c=g_locate;
		Vector3 pos= p.position;
		Vector3 vec=p.velocity;

		for(int k=0;k<difficulty;k++)
		{
			pos=pos.add(vec.mult(DELTA_T));
			int index= a.find_grid(position);
			if(index!=8)
			{
				c=a.neigh[index];
				trajectory.push_back(c);
				a=gameplay->Maps[c];
			}
			else
				trajectory.push_back(c);	
		}
		cout<<"returning traj"<<endl;
		return trajectory;
	}	
	else if(j==1)
	{	
		cout<<"bots enter"<<endl;
		//cout<<"p g locate"<<p.g_locate<<endl;
		bots b=gameplay->All_Bots[i];
		cout<<"b g locate"<<b.g_locate<<endl;
		if(b.velocity.mod()==0)
		{
			for(int k=0;k<difficulty;k++)
				trajectory.push_back(b.g_locate);
			return trajectory;
		}	
		grid a = gameplay->Maps[b.g_locate];
		//int b = new_location(p.up,p.right);
		int c=g_locate;
		
		Vector3 pos= b.position;
		Vector3 vec=b.velocity;

		for(int k=0;k<difficulty;k++)
		{
			pos=pos.add(vec.mult(DELTA_T));
			int index= a.find_grid(position);
			if(index!=8)
			{
				c=a.neigh[index];
				trajectory.push_back(c);
				a=gameplay->Maps[c];
			}
			else
				trajectory.push_back(c);	
		}
		return trajectory;
	}
}

vector<int> bots::projected_bullet(bullet b,int difficulty)	
{
	vector<int> trajectory;
	grid a = gameplay->Maps[b.g_locate];
	//int b = new_location(p.up,p.right);
	int c=g_locate;
	Vector3 pos= b.position;
	Vector3 vec=b.velocity;

	for(int k=0;k<difficulty;k++)
	{
		pos=pos.add(vec.mult(DELTA_T));
		int index= a.find_grid(position);
		if(index!=8)
		{
			c=a.neigh[index];
			trajectory.push_back(c);
			a=gameplay->Maps[c];
		}
		else
			trajectory.push_back(c);	
	}
	return trajectory;		
}

void bots::projected_collision()
{
	player_collision.clear();
	bots_collision.clear();
	bullet_collision.clear();

	vector<int> path=projected_path(id,1,difficulty_level);
	cout<<"projected path size "<<path.size()<<endl;
	vector<int> opponent_path;
	for(int i=0;i<field_of_view->view_players.size();i++)
	{
		int collide=-1;
		int a = field_of_view->view_players[i].id;
		opponent_path = projected_path(gameplay->All_Players[a].fwd,gameplay->All_Players[a].tang,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		player_collision.push_back(collide);	
	}

	for(int i=0;i<field_of_view->view_bots.size();i++)
	{
		int collide=-1;
		int a = field_of_view->view_bots[i].id;
		opponent_path = projected_path(gameplay->All_Bots[a].fwd,gameplay->All_Bots[a].tang,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		bots_collision.push_back(collide);	
	}
	for(int i=0;i<field_of_view->view_bullets.size();i++)
	{
		int collide=-1;
		opponent_path = projected_bullet(field_of_view->view_bullets[i],difficulty_level);
		for(int j=0;j<difficulty_level;j++)
		{
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		bullet_collision.push_back(collide);	
	}
}

vector<int> bots::projected_hit(vector<int> path)
{
	vector<int>hits;
	vector<int> opponent_path;
	for(int i=0;i<field_of_view->view_players.size();i++)
	{
		int collide=-1;
		int a = field_of_view->view_players[i].id;
		opponent_path = projected_path(gameplay->All_Players[a].fwd,gameplay->All_Players[a].tang,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		hits.push_back(collide);	
	}

	for(int i=0;i<field_of_view->view_bots.size();i++)
	{
		int collide=-1;
		int a = field_of_view->view_bots[i].id;
		opponent_path = projected_path(gameplay->All_Bots[a].fwd,gameplay->All_Bots[a].tang,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		hits.push_back(collide);	
	}
	return hits;
}

vector<int> bots::level_1_projected_path(int i,int j,int difficulty)
{
	vector<int> path = projected_path(id,1,difficulty_level);
	cout<<"on proj path cacl"<<endl;
	cout<<"proj path "<<path[0]<<endl;
	vector<int> level_1_path;
	for(int k=1;k<path.size();k++)
		level_1_path.push_back(path[k]);
	int last= path.size()-1;
	if(j==0)
	{
		cout<<"its a player"<<endl;
		cout<<"player index"<<i<<endl;
		player p=gameplay->All_Players[i];	
		if((p.velocity).mod()==0.0)
		{
			level_1_path.push_back(path[0]);
			cout<<"done with player"<<endl;
			return level_1_path;
		}
		else{
			// int a = gameplay->Maps[path.back()];
			Vector3 pos=p.position;
			Vector3 vec=p.velocity;
			float factor = (float)difficulty+1;
			level_1_path.push_back((gameplay->Maps[p.g_locate]).find_grid(pos.add(vec.mult(factor))));
			cout<<"done with player"<<endl;
			return level_1_path;	
		}
	}
	else if(j==1)
	{
		cout<<"its a bot"<<endl;
		cout<<"bot index"<<i<<endl;
		bots b=(gameplay->All_Bots)[i];
		if(b.velocity.mod()==0)
		{
			level_1_path.push_back(path[0]);
			return level_1_path;
		}
		else{
			// int a = gameplay->Maps[path.back()];
			Vector3 pos=b.position;
			Vector3 vec=b.velocity;
			grid g = gameplay->Maps[b.g_locate];
			float factor = (float)difficulty+1;
			level_1_path.push_back(g.find_grid(pos.add(vec.mult(factor))));
			return level_1_path;	
		}
	}		
}

vector<int> bots::level_1_projected_bullet(bullet b, int difficulty)
{
	vector<int> path = projected_path(id,1,difficulty_level);
	vector<int> level_1_path;
	for(int k=1;k<path.size();k++)
		level_1_path.push_back(path[k]);
	int last= path.size()-1;
	Vector3 pos=b.position;
	Vector3 vec=b.velocity;
	float factor = (float)difficulty+1;
	level_1_path.push_back((gameplay->Maps[b.g_locate]).find_grid(pos.add(vec.mult(factor))));
	return level_1_path;
}

void bots::level_1_projected_collision(int move_up,int move_right)
{
	level_1_player_collision.clear();
	level_1_bots_collision.clear();

	//save present movement directions
	int a=fwd;
	int b=tang;
	int c=g_locate;

	//change movement directions to input parameters
	fwd=move_up;
	tang=move_right;

	vector<int> path=level_1_projected_path(id,1,difficulty_level);
	cout<<"path "<<path.size()<<"	"<<path[0]<<endl;
	cout<<"done lvl proj path"<<endl;
	vector<int> opponent_path;
	for(int i=0;i<field_of_view->view_players.size();i++)
	{
		int collide=-1;
		int s = field_of_view->view_players[i].id;
		opponent_path = level_1_projected_path(gameplay->All_Players[s].fwd,gameplay->All_Players[s].tang,difficulty_level);
		for(int j=0;j<difficulty_level;j++)
		{
			cout<<"path  j  "<<path[j]<<" 		 "<<opponent_path[j]<<endl;
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		cout<<"checked"<<endl;
		level_1_player_collision.push_back(collide);	
	}

	for(int i=0;i<field_of_view->view_bots.size();i++)
	{
		int collide=-1;
		int s = field_of_view->view_bots[i].id;
		opponent_path = level_1_projected_path(gameplay->All_Bots[s].fwd,gameplay->All_Bots[s].tang,difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		level_1_bots_collision.push_back(collide);	
	}

	for(int i=0;i<field_of_view->view_bullets.size();i++)
	{
		int collide=-1;
		//int s = field_of_view->view_bots[i].id;
		opponent_path = level_1_projected_bullet(field_of_view->view_bullets[i],difficulty_level);
		
		for(int j=0;j<difficulty_level;j++)
		{
			if(gameplay->Maps[(path[j])].check_neighbour(opponent_path[j])!=-1)
			{
				collide=j;
				break;
			}	
		}
		level_1_bullet_collision.push_back(collide);	
	}
}

vector<int> bots::projected_shoot(int i)
{

	Vector3 d;
	Vector3 normal = gameplay->Maps[g_locate].normal;	
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

	bullet b(id,group_id,weaponry[current_weapon].ID,position,d,g_locate,normal);
	return projected_bullet(b,difficulty_level);

}
bool bots::search_boosts()
{
	if(field_of_view->view_boosts.size()==0)
	{
		state_stack.pop();
		return false;
	}
	/* tranisition to attack,dodgebullet,getpowerup,dodgeally,dodgeenemy,wander*/
		// health_pack_available is struct of healthpack type & pos
	else if ((field_of_view->view_boosts[0]).id==1)
	{
		if(lives==max_lives)
		{
			state_stack.pop();
			return false;
		}
	}
	else if((field_of_view->view_boosts[0]).id==2)
	{
		if(health==100)
		{
			state_stack.pop();
			return false;
		}
	}
	else
	{	
		for(int i=0;i<field_of_view->view_players.size();i++) //checking collsion with player
		{	
			if(player_collision[i]!=-1)
			{ 
				state_stack.push("dodge");
				return false;
			}
			
		}
			
		for(int i=0;i<field_of_view->view_bots.size();i++)	//checking collision with bots
		{
			if(bots_collision[i]!=-1)
			{
				state_stack.push("dodge");	
				return false;
			}
			
		}
		
		for(int i=0;i<field_of_view->view_bullets.size();i++)     //checking collision with bullets
		{	
			if(bullet_collision[i]!=-1)
			{
				state_stack.push("dodgebullet");
				return false;
			}
		}

		if(health>50)		// if sufficient health than go for power up
		{
			for(int i=0;i<field_of_view->view_armoury.size();i++)
			{
				state_stack.push("search_armoury");
				return false;
			}
		}


		// if(health>50)	//if sufficient health than go for attack
		// {
		// 	if(field_of_view->view_bots.size() >0)
		// 	{
		// 		state_stack.push("attack");
		// 		return false;
		// 	}
		// }
		int r=field_of_view->view_boosts[0].right;
		int u=field_of_view->view_boosts[0].up;
		bool me=true;
		
		if(field_of_view->view_boosts[0].id==2) 		//if it is health pack
		{
			bots alpha;
			for(int i=0;i<field_of_view->view_bots.size();i++)		//checking with all bots
			{
				alpha = gameplay->All_Bots[field_of_view->view_bots[i].id];
				if(group_id == alpha.group_id)	//check if ally
				{
					if(alpha.health ==100)
						continue;
					if(alpha.field_of_view->view_boosts.size()>0) //check if health pack is in his fov
					{
						int r1=alpha.field_of_view->view_boosts[0].right; 
						int u1=alpha.field_of_view->view_boosts[0].up;
						if(abs(r)+abs(u)> abs(r1)+abs(u1))				//check if he is closer
						{
								me=false;
								break;
						}
					}
				}
			}
		}

		if(me==true)		// I have to go
		{
			if(r!=0)
			{
				if(r>0)
					// ans.push_back(key_tap(right));
					ans.push_back(1);
				else 
					// ans.push_back(key_tap(left));
					ans.push_back(3);
			}
			else
			{
				if(u>0)
					// ans.push_back(key_tap(up));
					ans.push_back(0);
				else if(u<0)
				{	// ans.push_back(key_tap(down));
					ans.push_back(2);
				}		
			}
			state_stack.pop();
			return true;
		}
		else			// I don't have to go..
		{
			state_stack.pop();
			return false;
		}
	}
}

bool bots::search_armoury()
{
	if(field_of_view->view_armoury.size()==0)
	{
		state_stack.pop();
		return false;
	}
	else
	{
		for(int i=0;i<field_of_view->view_players.size();i++) //checking collsion with player 
		{	
			if(player_collision[i]!=-1)
			{ 
				state_stack.push("dodge");
				return false;
			}	
		}
			
		for(int i=0;i<field_of_view->view_bots.size();i++)	//checking collision with bots
		{
			if(bots_collision[i]!=-1)
			{
				state_stack.push("dodge");	
				return false;
			}	
		}

		if(health<50)		// if less health than go for health
		{
			for(int i=0;i<field_of_view->view_boosts.size();i++)
			{
				
				int r=field_of_view->view_boosts[0].right;
				int u=field_of_view->view_boosts[0].up;
				bool me=true;
				
				if(field_of_view->view_boosts[0].id==2) 		//if it is health pack
				{
					bots alpha;
					for(int i=0;i<field_of_view->view_bots.size();i++)		//checking with all bots
					{
						alpha = gameplay->All_Bots[field_of_view->view_bots[i].id];
						if(group_id == alpha.group_id)	//check if ally
						{
							if(alpha.health ==100)
								continue;
							if(alpha.field_of_view->view_boosts.size()>0) //check if health pack is in his fov
							{
								int r1=alpha.field_of_view->view_boosts[0].right; 
								int u1=alpha.field_of_view->view_boosts[0].up;
								if(abs(r)+abs(u)> abs(r1)+abs(u1))				//check if he is closer
								{
										me=false;
										break;
								}
							}
						}
					}
				}

				if(me==true)		// I have to go
				{
					if(r!=0)
					{
						if(r>0)
							// ans.push_back(key_tap(right));
							ans.push_back(1);
						else 
							// ans.push_back(key_tap(left));
							ans.push_back(3);
					}
					else
					{
						if(u>0)
							// ans.push_back(key_tap(up));
							ans.push_back(0);
						else if(u<0)
						{	// ans.push_back(key_tap(down));
							ans.push_back(2);
						}		
					}
					state_stack.pop();
					return true;
				}
			}
		}

		for(int i=0;i<field_of_view->view_bullets.size();i++)     //checking collision with bullets
		{	
			if(bullet_collision[i]!=-1)
			{
				state_stack.push("dodgebullet");
				return false;
			}
		}

		// if(health>50)	//if sufficient health than go for attack
		// {
		// 	if(field_of_view->view_bots.size() >0)
		// 	{
		// 		state_stack.push("attack");
		// 		return false;
		// 	}
		// }
		int r=field_of_view->view_armoury[0].right;
		int u=field_of_view->view_armoury[0].up;
		if(r!=0)
		{
			if(r>0)
				// ans.push_back(key_tap(right));
				ans.push_back(1);
			else 
				// ans.push_back(key_tap(left));
				ans.push_back(3);
		}
		else
		{
			if(u>0)
				// ans.push_back(key_tap(up));
				ans.push_back(0);
			else if(u<0)
				// ans.push_back(key_tap(down));
				ans.push_back(2);
		}
		state_stack.pop();
		return true;
	}
}

bool bots::dodge()
{
	int mini=100000;
	int temp,i0,j0;
	int allystart=100;
	int enemystart=80;
	for(int i=-1;i<2;i++)
	{
		for(int j=-1;j<2;j++)
		{
			temp=0;
			level_1_projected_collision(i,j);
			cout<<"lvl proj coll "<<endl;
			for(int k=0;k<field_of_view->view_players.size();k++)
			{
				if(level_1_player_collision[k] != -1)
				{
					if(group_id == gameplay->All_Players[field_of_view->view_players[k].id].group_id)
						temp += allystart/pow(2,level_1_player_collision[k] -1);
					else
						temp += enemystart/pow(2,level_1_player_collision[k] -1);	
				}
			}
			if(temp<mini)
			{
				mini=temp;
				i0=i;
				j0=j;
			}
			
		}
	}

	if(i0=-1)
		// ans.push_back(key_tap(down));
		ans.push_back(2);
	if(i0=1)
		// ans.push_back(key_tap(up));
		ans.push_back(0);
	if(j0=-1)
		// ans.push_back(key_tap(left));
		ans.push_back(3);
	if(j0=1)
		// ans.push_back(key_tap(right));
		ans.push_back(1);

	state_stack.pop();
	return true;
}

bool bots::dodgebullet()
{
	for(int i=0;i<field_of_view->view_players.size();i++) //checking collsion with player 
	{	
		if(player_collision[i]!=-1)
		{ 
			state_stack.push("dodge");
			return false;
		}	
	}
		
	for(int i=0;i<field_of_view->view_bots.size();i++)	//checking collision with bots
	{
		if(bots_collision[i]!=-1)
		{
			state_stack.push("dodge");	
			return false;
		}	
	}

	int mini=100000;
	int temp, i0,j0;
	int start=100;
	for(int i=-1;i<2;i++)
	{
		for(int j=-1;j<2;j++)
		{
			temp=0;
			level_1_projected_collision(i,j);

			for(int k=0;k<field_of_view->view_bullets.size();k++)
			{
				if(level_1_bullet_collision[k] != -1)
				{
					temp += start/pow(2,level_1_bullet_collision[k] -1);
				}
			}
			if(temp<mini)
			{
				mini=temp;
				i0=i;
				j0=j;
			}
			
		}
	}
	if(i0=-1)
		// ans.push_back(key_tap(down));
		ans.push_back(2);
	if(i0=1)
		// ans.push_back(key_tap(up));
		ans.push_back(0);
	if(j0=-1)
		// ans.push_back(key_tap(left));
		ans.push_back(3);
	if(j0=1)
		// ans.push_back(key_tap(right));
		ans.push_back(1);

	state_stack.pop();
	return true;
}

void bots::attack()
{
	int foe= 0;
	cout<<"view_players : "<<field_of_view->view_players.size()<<endl;
	cout<<"view_bots : "<<field_of_view->view_bots.size()<<endl;
	for(int i=0;i<field_of_view->view_players.size();i++)
	{
		if(group_id != (gameplay->All_Players[field_of_view->view_players[i].id]).group_id)
			foe++;
	}

	for(int i=0;i<field_of_view->view_bots.size();i++)
	{
		if(group_id != (gameplay->All_Bots[field_of_view->view_bots[i].id]).group_id)
			foe++;
	}
	if (foe==0)
	{
		// state_stack.pop();
		return;
	}
	else
	{
		cout<<"Enter toggle_weapon"<<endl;
		cout<<"current_weapon "<<current_weapon<<endl;
		//toggle	
		float curr_weapon=weaponry[current_weapon].last_shot - weaponry[current_weapon].shot_time;
		cout<<"curr_weapon "<<curr_weapon<<endl;
		float next_weapon=weaponry[(current_weapon+1)%weaponry.size()].last_shot - weaponry[(current_weapon+1)%weaponry.size()].shot_time;
		cout<<"next_weapon "<<next_weapon<<endl;
		if(weaponry.size()>1)
		{
			cout<<"do not enter"<<endl;
			if(current_weapon<weaponry.size()-1)
			{
				if(weaponry[current_weapon].damage>weaponry[current_weapon+1].damage)
				{
					if(curr_weapon<0 and next_weapon>=0)
						ans.push_back(8);
					else if(curr_weapon<0 and next_weapon<0)  //can't fire in this frame
					{
						if(next_weapon<curr_weapon)
							ans.push_back(8);
						return;
					}
				}
				else
				{
					if(next_weapon>=0)
						ans.push_back(8);
					else
					{
						if(curr_weapon<0)	//cant fire in this frame
						{
							if(next_weapon<curr_weapon)
								ans.push_back(8);
							return;
						}
					}
				}
			}
			else 
			{
				if(curr_weapon<0)
					ans.push_back(8);
			}
		}
		cout<<"exit toggle_weapon"<<endl;

		//fire
		int tempu=0,tempd=0,templ=0,tempr=0;
		int start=100;
		vector<int>shoot_up_collision = projected_hit(projected_shoot(0));    //function initializing four vectors 
		vector<int>shoot_right_collision = projected_hit(projected_shoot(1));
		vector<int>shoot_down_collision = projected_hit(projected_shoot(2));
		vector<int>shoot_left_collision = projected_hit(projected_shoot(3));

		cout<<"done shoots"<<endl;
		for(int i=0;i<field_of_view->view_players.size();i++)
		{	
			if(group_id == (gameplay->All_Players[field_of_view->view_players[i].id]).group_id)
			{	
				 if(shoot_up_collision[i] != -1)		//vector that gets initialized
				 	tempu -= start/pow(2,shoot_up_collision[i] -1);
				 if(shoot_down_collision[i] != -1)		//vector that gets initialized
				 	tempd -= start/pow(2,shoot_down_collision[i] -1);
				 if(shoot_left_collision[i] != -1)		//vector that gets initialized
				 	templ -= start/pow(2,shoot_left_collision[i] -1);
				 if(shoot_right_collision[i] != -1)		//vector that gets initialized
				 	tempr -= start/pow(2,shoot_right_collision[i] -1);
			}
			else
			{
				if(shoot_up_collision[i] != -1)		//vector that gets initialized
					tempu += start/pow(2,shoot_up_collision[i] -1);
				if(shoot_down_collision[i] != -1)		//vector that gets initialized
					tempd += start/pow(2,shoot_down_collision[i] -1);
				if(shoot_left_collision[i] != -1)		//vector that gets initialized
					templ += start/pow(2,shoot_left_collision[i] -1);
				if(shoot_right_collision[i] != -1)		//vector that gets initialized
					tempr += start/pow(2,shoot_right_collision[i] -1);	
			}
		}

		//bots attack
		int j= field_of_view->view_players.size();
		for(int i=0;i<field_of_view->view_bots.size();i++)
		{	
			if(group_id == (gameplay->All_Bots[field_of_view->view_bots[i].id]).group_id)
			{	
				 if(shoot_up_collision[i+j] != -1)		//vector that gets initialized
				 	tempu -= start/pow(2,shoot_up_collision[i+j] -1);
				 if(shoot_down_collision[i+j] != -1)		//vector that gets initialized
				 	tempd -= start/pow(2,shoot_down_collision[i+j] -1);
				 if(shoot_left_collision[i+j] != -1)		//vector that gets initialized
				 	templ -= start/pow(2,shoot_left_collision[i+j] -1);
				 if(shoot_right_collision[i+j] != -1)		//vector that gets initialized
				 	tempr -= start/pow(2,shoot_right_collision[i+j] -1);
			}
			else
			{
				if(shoot_up_collision[i+j] != -1)		//vector that gets initialized
					tempu += start/pow(2,shoot_up_collision[i+j] -1);
				if(shoot_down_collision[i+j] != -1)		//vector that gets initialized
					tempd += start/pow(2,shoot_down_collision[i+j] -1);
				if(shoot_left_collision[i+j] != -1)		//vector that gets initialized
					templ += start/pow(2,shoot_left_collision[i+j] -1);
				if(shoot_right_collision[i+j] != -1)		//vector that gets initialized
					tempr += start/pow(2,shoot_right_collision[i+j] -1);
			}

		}
			
		int maxi = max(max(tempu,tempd),max(templ,tempr));
		if(maxi>0)
		{
			
			if(maxi==tempu)
				// ans.push_back(key_tap(shoot_up));
				ans.push_back(4);
			else if(maxi==tempd)
				// ans.push_back(key_tap(shoot_down));
				ans.push_back(6);
			else if(maxi==templ)
				// ans.push_back(key_tap(shoot_left));
				ans.push_back(7);
			else
				// ans.push_back(key_tap(shoot_right));
				ans.push_back(5);
		}
		return;
	}

}	

bool bots::wander()
{
	projected_collision();
	for(int i=0;i<field_of_view->view_players.size();i++) //checking collsion with player 
	{	
		cout<<"checking player collision"<<endl;
		if(player_collision[i]!=-1)
		{ 
		    cout<<"collision with players"<<endl;
			state_stack.push("dodge");
			return false;
		}	
		cout<<"checked player collision"<<endl;
	}

	for(int i=0;i<field_of_view->view_bots.size();i++)	//checking collision with bots
	{
		if(bots_collision[i]!=-1)
		{
			// cout<<"collision with bots"<<endl;
			state_stack.push("dodge");	
			return false;
		}
		
	}

	for(int i=0;i<field_of_view->view_bullets.size();i++)     //checking collision with bullets
	{	
		if(bullet_collision[i]!=-1)
		{
			// cout<<"collision with bullets"<<endl;
			state_stack.push("dodgebullet");
			return false;
		}
	}

	// if(health>50)	//if sufficient health than go for attack
	// {
	// 	if(field_of_view->view_bots.size() >0)
	// 	{
	// 		state_stack.push("attack");
	// 		return false;
	// 	}
	// }
	if(health>50)
	{	if(field_of_view->view_armoury.size()>0)
		{
			state_stack.push("search_armoury");
			return false;
		}
	}

	if(health<50)
	{
		if(field_of_view->view_boosts.size() >0)
		{
			int r=field_of_view->view_boosts[0].right;
			int u=field_of_view->view_boosts[0].up;
			bool me=true;
			
			if(field_of_view->view_boosts[0].id==2) 		//if it is health pack
			{
				bots alpha;
				for(int i=0;i<field_of_view->view_bots.size();i++)		//checking with all bots
				{
					alpha = gameplay->All_Bots[field_of_view->view_bots[i].id];
					if(group_id == alpha.group_id)	//check if ally
					{
						if(alpha.health ==100)
							continue;
						if(alpha.field_of_view->view_boosts.size()>0) //check if health pack is in his fov
						{
							int r1=alpha.field_of_view->view_boosts[0].right; 
							int u1=alpha.field_of_view->view_boosts[0].up;
							if(abs(r)+abs(u)> abs(r1)+abs(u1))				//check if he is closer
							{
									me=false;
									break;
							}
						}
					}
				}
			}

			if(me==true)		// I have to go
			{
				if(r!=0)
				{
					if(r>0)
						// ans.push_back(key_tap(right));
						ans.push_back(1);
					else 
						// ans.push_back(key_tap(left));
						ans.push_back(3);
				}
				else
				{
					if(u>0)
						// ans.push_back(key_tap(up));
						ans.push_back(0);
					else if(u<0)
					{	// ans.push_back(key_tap(down));
						ans.push_back(2);
					}		
				}
				return true;
			}
		}
	}

	// ans.push_back(key_tap(up));
	ans.push_back(0);
	return true;
}

void bots::stack_change()
{	
	ans.clear();
	analyze_view();
	cout<<"analyze view done"<<endl;
	// cout<<"players "<<field_of_view->view_players.size()<<endl;
	// cout<<"bots "<<field_of_view->view_bots.size()<<endl;
	// cout<<"bullets "<<field_of_view->view_bullets.size()<<endl;
	// cout<<"health "<<field_of_view->view_boosts.size()<<"   "<<field_of_view->view_armoury.size()<<endl;
	if (wander()==true)
	{
		 cout<<"wander done starting to attack"<<endl;
		 attack();
		  cout<<"attack done"<<endl;
		// return(ans);
	}
	else
	{
		cout<<"wander done else part"<<endl;
		if(state_stack.top()=="dodge")
		{
			 cout<<"dodge start"<<endl;
			dodge();
		    cout<<"dodge end"<<endl;
		}
		else if(state_stack.top()=="dodgebullet")
		{
		    cout<<"dodgebullet start"<<endl;
			dodgebullet();
			cout<<"dodgebullet end"<<endl;
		}
		else if(state_stack.top()=="search_armoury")
		{
			cout<<"search_armoury start"<<endl;
			search_armoury();
			cout<<"search_armoury end"<<endl;
		}
		else if(state_stack.top()=="search_boosts")
		{
			cout<<"search_boosts start"<<endl;
			search_boosts();
			cout<<"search_boosts start"<<endl;
		}
		
		cout<<"else attack start"<<endl;
		attack();
		cout<<"else attack end"<<endl;
		// return(ans);
	}	
}

/*
movCount is length of return vector fill remaining spaces with nop
 */

// vector<game_map::key_tap>game_map::AI(int player_id,int movCount)
// {
// 	vector<key_tap> v;
// 	return v;
// }



