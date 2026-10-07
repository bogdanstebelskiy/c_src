#ifndef          _ADS1248_H_
#define          _ADS1248_H_

typedef struct ads1248_options_t ads1248_options_t;

ads1248_options_t* ads1248_init();
void ads1248_destroy(ads1248_options_t* ads);

#endif  //       _ADS1248_H_
