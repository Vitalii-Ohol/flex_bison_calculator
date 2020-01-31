#ifndef FUNC_H
#define FUNC_H

// Calculate factorial
double factorial(double n) 
{
	double x; double f=1;
	
	for (x=1; x<=n; x++) { 
		f *= x; 
	}
	
	return f;
}



// Function to convert binary to decimal 
double baseToDecimal(const char* str, int base) {
	//printf("HUUUUUUIIIIIII %s\n", str);
	
	int len = strlen(str);
	int b = 0;
	double dec_val = 0.0;
	double fraction = 0.0;

	char* ptr = strchr(str, '.');
	int whole_part = 0;
	if(ptr){
		//ptr = ptr+1;
		//printf("%s\n", ptr);
		whole_part = ptr-str;
		b = base;
		for(int i=whole_part; i<=len-1; i++){
			//printf("%c\n", str[i]);
			int digit = str[i]-48;
			if (digit>=0 && digit<base) {
				fraction += (digit*1.0)/b;
				b = b*base;
			}
		}
	}else{
		whole_part = len-1;
	}
	b = 1;
	for (int i=whole_part; i>=0; i--) {
		//printf("%c\n", str[i]);
		int digit = str[i]-48;
		if (digit>=0 && digit<base) {
			dec_val += digit*b;
			b = b * base;
		}
	}

	//dec_val += fraction;

	//printf("%.12f\n", dec_val);
	//printf("func  %f\n", dec_val);
	//printf("func  %f\n", fraction);
	//printf("func  %f\n", dec_val+fraction);
	return (double)(dec_val + fraction);
}

// Function to convert hexadecimal to decimal
double hexadecimalToDecimal(const char* str) {
	//printf("HUUUUUUIIIIIII   %s\n", str);
	int len = strlen(str);
	int base = 1;
	double dec_val = 0.0;
	double fraction = 0.0;


	char* ptr = strchr(str, '.');
	int whole_part = 0;
	if(ptr){
		//printf("%s\n", ptr);
		whole_part = ptr-str;
		base = 16;
		for(int i=whole_part; i<=len-1; i++){
			//printf("%c\n", str[i]);
			char c = str[i];

			if (c>='0' && c<='9') {
				fraction += ((c - 48)*1.0)/base;
				base = base * 16; 
			} else if (c>='A' && c<='F') {
				fraction += ((c - 55)*1.0)/base;
				base = base*16; 
			} else if (c>='a' && c<='f') {
				fraction += ((c - 87)*1.0)/base;
				base = base*16; 
			}
		}
	}else{
		whole_part = len-1;
	}

	base = 1;
	for (int i=whole_part; i>=0; i--) {
		//printf("%c\n", str[i]);
        char c = str[i];
		if (c>='0' && c<='9') {
			dec_val += (c - 48)*base;
			base = base * 16; 
		} else if (c>='A' && c<='F') {
			dec_val += (c - 55)*base;
			base = base*16; 
		} else if (c>='a' && c<='f') {
			dec_val += (c - 87)*base;
			base = base*16; 
		}
	}
	//dec_val += fraction;
	//printf("%.12f\n", dec_val);
	return dec_val + fraction;
}


#endif
