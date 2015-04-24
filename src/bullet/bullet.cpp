#include "bullet.h"

bullet::bullet()
{}

bullet::bullet(int player_ID_,int player_GroupID_,int weapon_ID_,Vector3 position_,Vector3 dir,int g_locate_,Vector3 normal_)
{
	bullet_id = bullet_counter+1;
	bullet_counter++;
	player_ID = player_ID_;
	player_GroupID = player_GroupID_;
	weapon_ID = weapon_ID_;
	position = position_;
	velocity=dir.mult(Weapon_set[weapon_ID_].speed);
	damage=Weapon_set[weapon_ID_].damage;
	g_locate=g_locate_;
	duration=0;
	normal = normal_;
}

void bullet::surface_constraint()
{
	float distance= gameplay->Maps[g_locate].distance_from_surface(position);
	position=position.add((gameplay->Maps[g_locate].normal).mult(-distance));              
}

void bullet::update()
{
	/*position update*/
	position = position.add(velocity.mult(DELTA_T));

	/*update grid and game map*/
	int index= gameplay->Maps[g_locate].find_grid(position);
	
	if(index<8)
	{
		/*remove bullet from present grid*/
		for(int i=0;i<gameplay->Maps[g_locate].Bullets.size();i++)
		{
			if(bullet_id==(gameplay->Maps[g_locate].Bullets[i].bullet_id))
			{
				gameplay->Maps[g_locate].Bullets.erase(gameplay->Maps[g_locate].Bullets.begin()+i);
				break;
			}
		}

		
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
		gameplay->Maps[g_locate].Bullets.push_back(*this);
		surface_constraint();
	
	}	

	/*update duration*/
	duration += 1;

	/*remove bullet if timespan completed*/
	// cout << "timespan" << Weapon_set[weapon_ID].timespan << endl;
	if(duration>Weapon_set[weapon_ID].timespan)
	{
		/*remove bullet from present grid*/
		for(int i=0;i<gameplay->Maps[g_locate].Bullets.size();i++)
		{
			if(bullet_id==(gameplay->Maps[g_locate].Bullets[i].bullet_id))
			{
				gameplay->Maps[g_locate].Bullets.erase(gameplay->Maps[g_locate].Bullets.begin()+i);
				break;
			}
		}
		/*remove bullet from map*/
		for(int i=0;i<gameplay->Bullets.size();i++)
		{
			if(bullet_id==gameplay->Bullets[i].bullet_id)
			{
				gameplay->Bullets.erase(gameplay->Bullets.begin()+i);
			}
		}
	}

}