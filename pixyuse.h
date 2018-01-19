#include <Pixy.h>

#include <SPI.h>


#include "filter.h"

class pixyuse{
	private:
	
	
	filter filterX[4] = {filter(10),filter(10),filter(10),filter(10)};
	filter filterY[4]= {filter(10),filter(10),filter(10),filter(10)};
	filter filterWidth[4] = {filter(10),filter(10),filter(10),filter(10)};
	filter filterHeight[4] = {filter(10),filter(10),filter(10),filter(10)};
	filter filterArea[4] = {filter(10),filter(10),filter(10),filter(10)};
	int detectflag = -1;

	
	public:
	Pixy pixy;
	pixyuse();
	void pixyFindBall();
	void pixyFindSidemark();
	void pixyFindDoormark();
	void getPixyPara(const int signature,float *pixyPara,int label);
	void paraInitial();
	void getDrivePara(int motion_type, int pixy_type); // 1 move, -1 rotate
	
	                  //x,y,wid,heigh,S
	float pixyPara1[5] = {0,0,0,0,0};
	float pixyPara2[5] = {0,0,0,0,0};
	float pixyPara3[5] = {0,0,0,0,0};
	float pixyPara4[5] = {0,0,0,0,0};
	                //x_error,y_error
	float error1[2] = {160,200};
	float error2[2] = {160,200};
	float error3[2] = {160,200};
	float error4[2] = {160,200};
	
	
	float BaseVec = 0;
	float DeltaVec = 0;
	float omega = 0;
	
	const int ballsignature = 1 ;
	const int sidemark_signature[4] = {2,3,4,5};
	const int door_signature[2] = {6,7};
};


pixyuse::pixyuse(){

	pixy.init();	
}

void pixyuse::paraInitial(){
	
	for (int j=0;j<5;j++){
	pixyPara1[j] = 180;  //signature i parameters
	pixyPara2[j] = 0;
	pixyPara3[j] = 0;
	pixyPara4[j] = 0;
	}
}
                                                        //label from 1-4
void pixyuse::getPixyPara(const int signature,float *pixyPara,int label){
	// int block_num = 100;
	// uint16_t blocks = pixy.getBlocks();
	// if (blocks){
		// for (int i=0;i<blocks;i++)
		// {
			// if (signature == pixy.blocks[i].signature)
			// {block_num = i;
			// detectflag = 0;   // we have detected an ideal object.
			// break;
			// }else{ detectflag = 1;}  // sth detected is not what we want
		// }
		// if (block_num !=100 && pixy.blocks[block_num].width * pixy.blocks[block_num].height>50 )
		// {	label = label - 1; 
			// pixyPara[0]=filterX[label].filteroutcome(pixy.blocks[block_num].x);
			// pixyPara[1]=filterY[label].filteroutcome(pixy.blocks[block_num].y);
			// pixyPara[2]=filterWidth[label].filteroutcome(pixy.blocks[block_num].width);
			// pixyPara[3]=filterHeight[label].filteroutcome(pixy.blocks[block_num].height);
			// pixyPara[4]=filterArea[label].filteroutcome(pixy.blocks[block_num].height * pixy.blocks[block_num].width );

		// } 
	// }else {detectflag = -1;}  // nothing is detected.

}

void pixyuse::pixyFindBall(){
	getPixyPara(ballsignature,pixyPara1,1);
    // error1[0] = pixyPara1[0] - 160;  // error to central line
	// error1[1] = pixyPara1[1] - 0;    //distance from bottom line
}

void pixyuse::pixyFindSidemark(){
	getPixyPara(sidemark_signature[0],pixyPara1,1);
    error1[0] = pixyPara1[0] - 160;  // error to central line
	error1[1] = pixyPara1[1] - 0;    //distance from bottom line
	
	getPixyPara(sidemark_signature[1],pixyPara2,2);
    error2[0] = pixyPara2[0] - 160;  // error to central line
	error2[1] = pixyPara2[1] - 0;    //distance from bottom line
	
	getPixyPara(sidemark_signature[2],pixyPara3,3);
    error3[0] = pixyPara3[0] - 160;  // error to central line
	error3[1] = pixyPara3[1] - 0;    //distance from bottom line
	
    getPixyPara(sidemark_signature[3],pixyPara4,4);
    error4[0] = pixyPara4[0] - 160;  // error to central line
	error4[1] = pixyPara4[1] - 0;    //distance from bottom line
	
}

void pixyuse::pixyFindDoormark(){
	getPixyPara(door_signature[0],pixyPara1,1);
    error1[0] = pixyPara1[0] - 160;  // error to central line
	error1[1] = pixyPara1[1] - 0;    //distance from bottom line
	
	getPixyPara(door_signature[1],pixyPara2,2);
    error2[0] = pixyPara2[0] - 160;  // error to central line
	error2[1] = pixyPara2[1] - 0;    //distance from bottom line	
	
}

#define Kr  0.5
void pixyuse::getDrivePara( int motion_type, int pixy_type){
	switch(motion_type * pixy_type){
	case 1:
		if (error1[1] > 50){
			DeltaVec = Kr * error1[0];
		    BaseVec = 60;
		}else {
				DeltaVec = Kr * error1[0];
				BaseVec = 40;
			}
	break;
	
	case -1:
		if (abs(error1[0])>80){
			omega = 100 * (error1[0]/abs(error1[0]));			
		} else if (abs(error1[0])>20)
			omega = 40 * (error1[0]/abs(error1[0]));
		else {omega = 0;}
		
	
	break;
	
	case 2:
	break;
	
	case 3: 

	break;
	}
	
	
}



