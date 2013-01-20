#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
 
void current_utc_time(struct timespec *ts) {
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
}
 
extern "C" {
long to_dec( unsigned long value, char * buffer, long buffer_len );
}

long to_dec_c( unsigned long value, char * buffer, long buffer_len );

const long nsec_per_sec = 1000000000;

long int diff_timespec(struct timespec &start, struct timespec &end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = nsec_per_sec + end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}

	return (temp.tv_sec * nsec_per_sec) + temp.tv_nsec;
}

int main( int argc, char ** argv )
{
	struct timespec start, end;
	char buffer[32] = {0};
	const int vals = 10000000;

	unsigned long *val = new unsigned long[vals];

	long int duration;

	long test_len;
	test_len = to_dec_c( 0xDEADBEEFDEADBEEF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 1 0xDEADBEEFDEADBEEF = %s\n", buffer );

	test_len = to_dec( 0xDEADBEEFDEADBEEF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 2 0xDEADBEEFDEADBEEF = %s\n", buffer );

	test_len = to_dec( 0xFFFFFFFFFFFFFFFF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 3 0xFFFFFFFFFFFFFFEF = %s\n", buffer );

	test_len = to_dec_c( 0xFFFFFFFFFFFFFFFF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 4 0xFFFFFFFFFFFFFFEF = %s\n", buffer );

	test_len = to_dec( (unsigned long)0xFFFFFFFF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 5 0xFFFFFFFF = %s\n", buffer );

	test_len = to_dec_c( (unsigned long)0xFFFFFFFF, buffer, 32 );
	buffer[test_len] = 0x00;
	printf( "Test 6 0xFFFFFFFF = %s\n", buffer );

	printf( "Array setup..." ); fflush(NULL);

	for( int x = 0; x < vals; x++ ) {
		val[x] = x;
	}

	printf( "done\n" );

	current_utc_time( &start );

	for( int x = 0; x < vals ; x++ ) {
		long len = to_dec( val[x], buffer, 32 );
	}

	current_utc_time( &end );

	duration = diff_timespec( start, end );

	printf( "to_dec, source is array\n" );
	printf( "Completed in %8.8f sec (%ld nsec avg)\n\n", (double)duration/nsec_per_sec, duration/vals );


	current_utc_time( &start );

	for( int x = 0; x < vals ; x++ ) {
		long len = to_dec( x, buffer, 32 );
	}

	current_utc_time( &end );

	duration = diff_timespec( start, end );

	printf( "to_dec, source is calculated\n" );
	printf( "Completed in %8.8f sec (%ld nsec avg)\n\n", (double)duration/(double)nsec_per_sec, duration/vals );

	current_utc_time( &start );

	for( int x = 0; x < vals ; x++ ) {
		long len = to_dec_c( val[x], buffer, 32 );
	}

	current_utc_time( &end );

	duration = diff_timespec( start, end );

	printf( "to_dec_c, source is array\n" );
	printf( "Completed in %8.8f sec (%ld nsec avg)\n\n", (double)duration/(double)nsec_per_sec, duration/vals );


	current_utc_time( &start );

	for( int x = 0; x < vals ; x++ ) {
		long len = to_dec_c( x, buffer, 32 );
	}


	current_utc_time( &end );
	duration = diff_timespec( start, end );

	printf( "to_dec_c, source is calculated\n" );
	printf( "Completed in %8.8f sec (%ld nsec avg)\n\n", (double)duration/(double)nsec_per_sec, duration/vals );


	return 0;
}

