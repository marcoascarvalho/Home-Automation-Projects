
#define LIGHT_MINUTES           5
#define DAY_TRESHOLD            200
#define NIGHT_TRESHOLD          100
#define IS_DAY                  0x01
#define IS_NIGHT                0x02
#define IS_EVENING_MORNING      0x03


// library interface description
class lightsensorClass //: public Stream
{
  public:
    // constructors:
    lightsensorClass();
    
    byte CheckCurrentLight();


  private:
    static uint16_t DayCount;
    static uint16_t NightCount;
    
    bool CheckForDay(int CurrentLight);
    bool CheckForNight(int CurrentLight);

};

extern lightsensorClass lightsensor;