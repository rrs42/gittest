

long to_dec_c( unsigned long val, char * buffer, long buffer_len )
{
	int x,j,len;

	for( x = 0; val > 0; x++,val/=10 )
	{
		char digit = val % 10;
		buffer[x] = digit + '0';
	}

	len=x;

	for( j = 0; x >= len/2 ; x--,j++ ) {
		char tmp  = buffer[j];
		buffer[j] = buffer[x-1];
		buffer[x-1] = tmp;
	}

	return len;
}
