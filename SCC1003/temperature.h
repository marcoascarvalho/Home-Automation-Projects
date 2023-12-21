
#define BASE_TEMPERATURE 5

// library interface description
class temperatureClass //: public Stream
{
  public:
    // constructors:
    temperatureClass();
    
    int GetTemperatureValue();


  private:


};

extern temperatureClass temperature;