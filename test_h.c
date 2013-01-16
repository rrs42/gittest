#include <stdio.h>
#include <time.h>
#include <math.h>

extern "C" {
long to_dec( unsigned long value, char * buffer, long buffer_len );
}

/*
struct timespec diff(struct timespec &start, struct timespec &end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
*/

long int diff_timespec(struct timespec &start, struct timespec &end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}

	return (temp.tv_sec * 1000000000) + temp.tv_nsec;
}

int main( int argc, char ** argv )
{
	struct timespec start, end;
	char buffer[32] = {0};
	const int vals = 10000000;

	unsigned long *val = new unsigned long[vals];

	long int duration;

	long test_len;
	test_len = to_dec( 0xDEADBEEFDEADBEEF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 1 0xDEADBEEFDEADBEEF = %s\n", buffer );

	test_len = to_dec( 0xFFFFFFFFFFFFFFFF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 1 0xFFFFFFFFFFFFFFEF = %s\n", buffer );

	test_len = to_dec( (unsigned long)0xFFFFFFFF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 3 0xFFFFFFFF = %s\n", buffer );

	printf( "Array setup..." ); fflush(NULL);

	for( int x = 0; x < vals; x++ ) {
		val[x] = x;
	}

	printf( "done\n" );

	clock_gettime( CLOCK_MONOTONIC, &start );

	for( int x = 0; x < vals ; x++ ) {
		long len = to_dec( val[x], buffer, 32 );
	}

	clock_gettime( CLOCK_MONOTONIC, &end );

	duration = diff_timespec( start, end );

	printf( "to_dec, source is array\n" );
	printf( "Completed in %8.8f sec (%d nsec avg)\n\n", (double)duration/1000000000.0, duration/vals );


	clock_gettime( CLOCK_MONOTONIC, &start );

	for( int x = 0; x < vals ; x++ ) {
		long len = to_dec( x, buffer, 32 );
	}

	clock_gettime( CLOCK_MONOTONIC, &end );

	duration = diff_timespec( start, end );

	printf( "to_dec, source is calculated\n" );
	printf( "Completed in %8.8f sec (%d nsec avg)\n\n", (double)duration/1000000000.0, duration/vals );

	return 0;
}

