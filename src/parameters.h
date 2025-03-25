#define threedots_corX          240
#define threedots_corY          300
#define threedots_distance      20
#define threedots_R             5

double Throttle = 0;
double Brake = 0;
double HV = 0;
double Lat = 0;
double Lon = 0;
int INT_V = 0;
char velocity[4] = {'0','0','0','0'};

String int2v(int num){
    if(num > 999) return "---";
    char v[4];
    v[3] = '\0';
    v[2] = (char)(num%10+48);
    v[1] = (char)(num%100/10+48);
    v[0] = (char)(num/100+48);
    return v;
  };
