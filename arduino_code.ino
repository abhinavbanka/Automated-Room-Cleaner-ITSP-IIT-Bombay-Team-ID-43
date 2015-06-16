# ITSP-Team-43
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ESC&BRUSHLESS MOTOR VACUUMING$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <Servo.h>//for esc
Servo vacuum;//esc&brushless motor
//function to arm the esc
void arm(){ 
 // arm the speed controller, modify as necessary for your ESC  
 setSpeed(0); 
 delay(1000); //delay 1 second,  some speed controllers may need longer
}
void setSpeed(int speed){
 // speed is from 0 to 100 where 0 is off and 100 is maximum speed
 //the following maps speed values of 0-100 to angles from 0-180,
 // some speed controllers may need different values, see the ESC instructions
 int angle = map(speed, 0, 100, 0, 180);
 vacuum.write(angle);    
}
//starting the brushless motor at maximum speed
//attach esc signal wire at pin 13
void start_vacuuming()
{
  vacuum.attach(13);
  arm();
  int speed;
  for(speed = 0; speed <= 100; speed +=20) 
  {
   setSpeed(speed);
   delay(1000);
  }
}
//gradually stop the brushless motor
void stop_vacuuming()
{
  int speed;
  for(speed =100; speed>=0; speed -=5) 
  {
   setSpeed(speed);
   delay(1000);
  }
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Wiping motors$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
int wiping_motor_left;
int wiping_motor_right;
//start the wiping motors

void start_wiping()
{
  digitalWrite(wiping_motor_left,HIGH);
  digitalWrite(wiping_motor_right,HIGH);
}
void stop_wiping()
{
  digitalWrite(wiping_motor_left,LOW);
  digitalWrite(wiping_motor_right,LOW);
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$sensor$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//simply call sensor_name.getdistance() to get the integer distance in centimetres 

class sensor 
{
  public:
  int trigPin;
  int echoPin;
  //Returns distance in centemetres
  int getdistance()
  {
    digitalWrite(trigPin,LOW);
    delayMicroseconds(3);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    long duration=pulseIn(echoPin,HIGH);
    return((duration/2)/29.1);
  }
  sensor(int trig,int echo){trigPin=trig;echoPin=echo;}
};
//sensors defination
//format is (trigPin,echopin)
//edit the numbers on adding to the code
sensor bottom(1,2);
sensor front(3,4);
sensor front_right(5,7);
sensor left(6,8);
sensor right(9,10);
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$turning the bot$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Circuit Connections //
//through L293D
//connect each motor in such a way that motopin_righ1 is HIGH and motorpin_right2 is LOW then the motor should run forward
//similarly for motorpin_left
int motorpin_left1=1;
int motorpin_left2=2;
int motorpin_right1=3;
int motorpin_right2=4;
//PRECONDITOION
//if right==1 turn right else turn left
void turn_bot(bool right)
{
  //delay to stop the bot before it turns
  delay(1000); //trial and error 
  if(right==1)
  {
    //keep right wheels moving forward
    digitalWrite(motorpin_right1,HIGH);
    digitalWrite(motorpin_right2,LOW);
    //make left wheels move backward
    digitalWrite(motorpin_left1,LOW);
    digitalWrite(motorpin_left2,HIGH);
  }
  else
  {
    //keep left wheels moving forward
    digitalWrite(motorpin_left1,HIGH);
    digitalWrite(motorpin_left2,LOW);
    //make right wheels move backward
    digitalWrite(motorpin_right1,LOW);
    digitalWrite(motorpin_right2,HIGH);
  }
  delay(1000);//trial and error till the bot turns through 180 degree
  digitalWrite(motorpin_right1,LOW);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,LOW);
  digitalWrite(motorpin_left2,LOW);
}
void turn_90(bool right)
{
  //delay to stop the bot before it turns
  delay(1000); //trial and error 
  if(right==1)
  {
    //keep right wheels moving forward
    digitalWrite(motorpin_right1,HIGH);
    digitalWrite(motorpin_right2,LOW);
    //make left wheels move backward
    digitalWrite(motorpin_left1,LOW);
    digitalWrite(motorpin_left2,HIGH);
  }
  else
  {
    //keep left wheels moving forward
    digitalWrite(motorpin_left1,HIGH);
    digitalWrite(motorpin_left2,LOW);
    //make right wheels move backward
    digitalWrite(motorpin_right1,LOW);
    digitalWrite(motorpin_right2,HIGH);
  }
  delay(1000);//trial and error till the bot turns through 90 degree
  digitalWrite(motorpin_right1,LOW);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,LOW);
  digitalWrite(motorpin_left2,LOW);
}
void move_steps(int steps)
{
  //move all four wheels in forward direction
  digitalWrite(motorpin_right1,HIGH);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,HIGH);
  digitalWrite(motorpin_left2,LOW);
  delay(steps*1000);//trial and error
  digitalWrite(motorpin_right1,LOW);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,LOW);
  digitalWrite(motorpin_left2,LOW);
}
bool is_wall_on_right()//returns true if wall is near on right side of the bot
{
  if((left.getdistance())>(right.getdistance()))
  {
    return true;
  }
  else{return false;}
}
bool is_wall_close(bool right_side)//takes the boolean variable whether the wall is on right or left and returns whether it is really close(less than 8cm trial and error)
{
  if(right_side)
  {
    if(right.getdistance()<=8)//trial and error
    {
      return true;
    }
    else return false;
  }
  else
  {
    if(left.getdistance()<=8)//trial and error
    {
      return true;
    }
    else return false;
  }
}
bool move_bot(bool side_right)//takes the boolean if true, checks on right side for the wall else left side
//returns 1 if wall is just close on given side
//else returns 0
//makes the bot move till it sees next obstacle at a distance of 5cm trial and error
{
  //move all four wheels in forward direction
  digitalWrite(motorpin_right1,HIGH);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,HIGH);
  digitalWrite(motorpin_left2,LOW);
  bool check_close=true;
  do
  {
    if(check_close)
    {check_close=((is_wall_close(side_right))&&(check_close));}
  }while((front.getdistance())>5);//trial and error;
  digitalWrite(motorpin_right1,LOW);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,LOW);
  digitalWrite(motorpin_left2,LOW);
  return check_close;
}
void move_forward()
{
  digitalWrite(motorpin_right1,HIGH);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,HIGH);
  digitalWrite(motorpin_left2,LOW);
  do{} while((front.getdistance())>10);//trial and error
  digitalWrite(motorpin_right1,LOW);
  digitalWrite(motorpin_right2,LOW);
  digitalWrite(motorpin_left1,LOW);
  digitalWrite(motorpin_left2,LOW);
  return;
}   

void setup() {
  pinMode(wiping_motor_left,OUTPUT);
  pinMode(wiping_motor_right,OUTPUT);
  pinMode(bottom.trigPin,OUTPUT);
  pinMode(bottom.echoPin,INPUT);
  pinMode(front.trigPin,OUTPUT);
  pinMode(front.echoPin,INPUT);
  pinMode(front_right.trigPin,OUTPUT);
  pinMode(front_right.echoPin,INPUT);
  pinMode(left.trigPin,OUTPUT);
  pinMode(left.echoPin,INPUT);
  pinMode(right.trigPin,OUTPUT);
  pinMode(right.echoPin,INPUT);
  pinMode(motorpin_left1,OUTPUT);
  pinMode(motorpin_left2,OUTPUT);
  pinMode(motorpin_right1,OUTPUT);
  pinMode(motorpin_right2,OUTPUT);
  Serial.begin(9600);
  pinMode(45,INPUT);//pin for taking input whether vacuuming or wiping is to be done trial and error
  if(digitalRead(45)== HIGH)//trial and error
  {
    //vacuuming
    start_vacuuming();
    bool init_on_right=is_wall_on_right();//is true if wall is on right initially,else false
    int i;
    int dist[100]={-1};
    int dmax=0;
    for(i=0;;i++)//i stands for row/column number of the zigzag path
    {
      dist[i]=front.getdistance();
      if(dist[i]>dmax){dmax=dist[i];}
      if(init_on_right)
      {
        if(move_bot(i%2)){break;}
        else
        {
          turn_bot(i%2);
        }
      }
      else
      {
        if(move_bot(!(i%2))){break;}
        else
        {
          turn_bot(!(i%2));
        }
      }
    }
   int obstacles[100]={-1};
   int x=0;
   for(int z=0;z<101;z++)
   {
     if(dist[z]==-1){break;}
     else if((dmax-dist[z])>85)//trial and error
     {
       obstacles[x]=z;
       x++;
     }
   }
   int start_row[50]={-1};int end_row[50]={-1};
   int s=0;
   int e=0;
   for(int z=0;z<101;z++)
   {
     if(obstacles[z]==-1)
     {
       if(z!=0){end_row[e]=obstacles[z-1];e++;}
       break;
     }
     if(z==0){start_row[s]=obstacles[z];s++;}
     else if ((obstacles[z]-obstacles[z-1])>1)
     {
       start_row[s]=obstacles[z];s++;
       end_row[e]=obstacles[z-1];e++;
     }
   }
   int reference=i;//
   
   
   if(init_on_right)//initially wall is on the right of the bot
   {
     if(i%2)//bot is on the initial side of the room
     {
       turn_90(false);//turn 90 degree left
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(!(end_row[j]%2))//if ending row/column no of obstacle is even
         {
             move_steps(reference-(end_row[j]));
             turn_bot(false);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(!(q%2));}
             }
             turn_90(false);
         }
       }
       move_forward();
       turn_90(false);
       move_forward();
       turn_90(false);
       reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(!(start_row[j]%2))
         {
           move_steps(start_row[j]-reference);
           turn_bot(false);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(!(q%2));}
           }
           turn_90(false);
         }
       }
     }
     else
     {
       turn_90(true);
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(end_row[j]%2)//if ending row/column no of obstacle is odd
         {
             move_steps(reference-(end_row[j]));
             turn_bot(true);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(!(q%2));}
             }
             turn_90(true);
         }
       }
         move_forward();
         turn_90(true);
         move_forward();
         turn_90(true);
         reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(start_row[j]%2)
         {
           move_steps(start_row[j]-reference);
           turn_bot(true);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(!(q%2));}
           }
           turn_90(true);
         }
       }
     }
   }
   else
   {
      if(i%2)//bot is on the initial side of the room
     {
       turn_90(true);//turn 90 degree left
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(!(end_row[j]%2))//if ending row/column no of obstacle is even
         {
             move_steps(reference-(end_row[j]));
             turn_bot(true);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(q%2);}
             }
             turn_90(true);
         }
       }
       move_forward();
       turn_90(true);
       move_forward();
       turn_90(true);
       reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(!(start_row[j]%2))
         {
           move_steps(start_row[j]-reference);
           turn_bot(true);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(q%2);}
           }
           turn_90(true);
         }
       }
     }
     else
     {
       turn_90(false);
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(end_row[j]%2)//if ending row/column no of obstacle is odd
         {
             move_steps(reference-(end_row[j]));
             turn_bot(false);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(q%2);}
             }
             turn_90(false);
         }
       }
         move_forward();
         turn_90(false);
         move_forward();
         turn_90(false);
         reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(start_row[j]%2)
         {
           move_steps(start_row[j]-reference);
           turn_bot(false);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(q%2);}
           }
           turn_90(false);
         }
       }
     }
   }
   stop_vacuuming();
  }
  
  if(digitalRead(45)==LOW)//trial and error
  {
    //Wiping
    start_wiping();
    bool init_on_right=is_wall_on_right();//is true if wall is on right initially,else false
    int i;
    int dist[100]={-1};
    int dmax=0;
    for(i=0;;i++)//i stands for row/column number of the zigzag path
    {
      dist[i]=front.getdistance();
      if(dist[i]>dmax){dmax=dist[i];}
      if(init_on_right)
      {
        if(move_bot(i%2)){break;}
        else
        {
          turn_bot(i%2);
        }
      }
      else
      {
        if(move_bot(!(i%2))){break;}
        else
        {
          turn_bot(!(i%2));
        }
      }
    }
   int obstacles[100]={-1};
   int x=0;
   for(int z=0;z<101;z++)
   {
     if(dist[z]==-1){break;}
     else if((dmax-dist[z])>85)//trial and error
     {
       obstacles[x]=z;
       x++;
     }
   }
   int start_row[50]={-1};int end_row[50]={-1};
   int s=0;
   int e=0;
   for(int z=0;z<101;z++)
   {
     if(obstacles[z]==-1)
     {
       if(z!=0){end_row[e]=obstacles[z-1];e++;}
       break;
     }
     if(z==0){start_row[s]=obstacles[z];s++;}
     else if ((obstacles[z]-obstacles[z-1])>1)
     {
       start_row[s]=obstacles[z];s++;
       end_row[e]=obstacles[z-1];e++;
     }
   }
   int reference=i;//
   
   
   if(init_on_right)//initially wall is on the right of the bot
   {
     if(i%2)//bot is on the initial side of the room
     {
       turn_90(false);//turn 90 degree left
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(!(end_row[j]%2))//if ending row/column no of obstacle is even
         {
             move_steps(reference-(end_row[j]));
             turn_bot(false);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(!(q%2));}
             }
             turn_90(false);
         }
       }
       move_forward();
       turn_90(false);
       move_forward();
       turn_90(false);
       reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(!(start_row[j]%2))
         {
           move_steps(start_row[j]-reference);
           turn_bot(false);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(!(q%2));}
           }
           turn_90(false);
         }
       }
     }
     else
     {
       turn_90(true);
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(end_row[j]%2)//if ending row/column no of obstacle is odd
         {
             move_steps(reference-(end_row[j]));
             turn_bot(true);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(!(q%2));}
             }
             turn_90(true);
         }
       }
         move_forward();
         turn_90(true);
         move_forward();
         turn_90(true);
         reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(start_row[j]%2)
         {
           move_steps(start_row[j]-reference);
           turn_bot(true);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(!(q%2));}
           }
           turn_90(true);
         }
       }
     }
   }
   else
   {
      if(i%2)//bot is on the initial side of the room
     {
       turn_90(true);//turn 90 degree left
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(!(end_row[j]%2))//if ending row/column no of obstacle is even
         {
             move_steps(reference-(end_row[j]));
             turn_bot(true);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(q%2);}
             }
             turn_90(true);
         }
       }
       move_forward();
       turn_90(true);
       move_forward();
       turn_90(true);
       reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(!(start_row[j]%2))
         {
           move_steps(start_row[j]-reference);
           turn_bot(true);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(q%2);}
           }
           turn_90(true);
         }
       }
     }
     else
     {
       turn_90(false);
       for(int j=e-1;j>=0;j--)//covers all the remained areas on the same side where room ended
       {
         if(end_row[j]%2)//if ending row/column no of obstacle is odd
         {
             move_steps(reference-(end_row[j]));
             turn_bot(false);
             reference=start_row[j];
             for(int q=end_row[j];q>=start_row[j];q--)
             {
               move_forward();
               if(q!=start_row[j]){turn_bot(q%2);}
             }
             turn_90(false);
         }
       }
         move_forward();
         turn_90(false);
         move_forward();
         turn_90(false);
         reference=0;
       for(int j=0;j<s;j++)//to cover all the remaining areas on the other side
       {
         if(start_row[j]%2)
         {
           move_steps(start_row[j]-reference);
           turn_bot(false);
           reference=end_row[j];
           for(int q=start_row[j];q<=end_row[j];q++)
           {
             move_forward();
             if(q!=end_row[j]){turn_bot(q%2);}
           }
           turn_90(false);
         }
       }
     }
   }
   stop_wiping();
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
