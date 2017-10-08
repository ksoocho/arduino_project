
#define CAPCT 10



class Punt{
public:
    float p[3];
    boolean rapidMotion;
    boolean absolutes;
    int spindle;
	float feedRate;				// limit de velocitat actual en mm / min
	boolean setCoordinates;
	boolean delay;



    Punt(){
    }////



    void set(float lp[], boolean labsolutes, boolean lrapidMotion, int lspindle, float lfeedRate, boolean lsetCoordinates, boolean ldelay){
        for(int i = 0; i<3; i++) p[i] = lp[i];
        absolutes = labsolutes;
        rapidMotion = lrapidMotion;
        spindle = lspindle;   
		feedRate = lfeedRate;
		setCoordinates = lsetCoordinates;
		delay = ldelay;
    }////



    void get(float lp[], boolean *labsolutes, boolean *lrapidMotion, int *lspindle, float *lfeedRate, boolean *lsetCoordinates, boolean *ldelay){
        for(int i = 0; i<3; i++) lp[i] = p[i];
        *lrapidMotion = rapidMotion;
        *labsolutes = absolutes;
        *lspindle = spindle;
		*lfeedRate = feedRate;
		*lsetCoordinates = setCoordinates;
		*ldelay = delay;
    }////

};////////////////////////// end class Punt



class PuntsList{
public:
    Punt punt[CAPCT];
    int head;
    int tail;

    PuntsList(){  
        head = 0;  
        tail = 0; 
    }////

    boolean isFull() {
        return( ( (tail+1) % CAPCT ) == head );
    }////

    boolean isEmpty(){  
        return( head == tail );                  
    }////

    void resetPunts(){  
        head = 0; 
		tail = 0;
    }  ////



    boolean pushPunt(float lp[], boolean labsolutes, boolean lrapidMotion, int lspindle, float lfeedRate, boolean lsetCoordinates, boolean ldelay){  // si la llista és plena retorna false
        if(isFull()){
            return(false);
        }
        else{
            punt[tail].set(lp, labsolutes, lrapidMotion, lspindle, lfeedRate, lsetCoordinates, ldelay);

            tail++;
            tail = tail % CAPCT;
            return(true);
        }
    }////



    boolean popPunt(float lp[], boolean *labsolutes, boolean *lrapidMotion, int *lspindle, float *lfeedRate, boolean *lsetCoordinates, boolean *ldelay){  // si la cua és buida retorna false
        if(isEmpty ()){
            return(false);
        }
        else{
            punt[head].get(lp, labsolutes, lrapidMotion, lspindle, lfeedRate, lsetCoordinates, ldelay);

            head++;
            head = head % CAPCT;
            return(true);
        }
    }////


private:
};////////////////////////// end class PuntsList




