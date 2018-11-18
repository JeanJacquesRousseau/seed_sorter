/*
 * Seed.h
 * Created : 2018-17-11 23:03:22
 * Author: David Patry
 */
 

#ifndef __SEED_H__
#define __SEED_H__

#define HEIGHT	600//768
#define WIDTH	800//1024

class Seed
{
	public:
		Seed();
		~Seed();
		void setCenter(float x, float y);
		float getPosX();
		float getPosY();
		
	protected:
	private:
		float center_x;
		float center_y;
		//Point2f point;	
};





#endif


