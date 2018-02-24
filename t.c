#define MAX_TITKOS 4096
#define OLVASAS_BUFFER 256
#define KULCS_MERET 5
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>

double
atlagos_szohossz (const char *titkos, int titkos_meret)
{
  int sz = 0;
  for (int i = 0; i < titkos_meret; ++i)
    if (titkos[i] == ' ')
      ++sz;

  return (double) titkos_meret / sz;
}

int
tiszta_lehet (const char *titkos, int titkos_meret)
{
  // a tiszta szoveg valszeg tartalmazza a gyakori magyar szavakat
  // illetve az átlagos szóhossz vizsgálatával csökkentjük a
  // potenciális töréseket

  double szohossz = atlagos_szohossz (titkos, titkos_meret);

  return szohossz > 6.0 && szohossz < 9.0
    && ( strcasestr (titkos, " hogy ") || strcasestr (titkos, " nem ") 
	|| strcasestr (titkos, " az ") || strcasestr (titkos, " ha ") );

}

void
exor (const char kulcs[], int kulcs_meret, char titkos[], int titkos_meret)
{

  int kulcs_index = 0;

  for (int i = 0; i < titkos_meret; ++i)
    {

      titkos[i] = titkos[i] ^ kulcs[kulcs_index];
      kulcs_index = (kulcs_index + 1) % kulcs_meret;

    }

}

int
exor_tores (const char kulcs[], int kulcs_meret, char titkos[],
	    int titkos_meret)
{

  exor (kulcs, kulcs_meret, titkos, titkos_meret);

  return tiszta_lehet (titkos, titkos_meret);

}

int
main (void)
{

  char kulcs[KULCS_MERET];
  char titkos[MAX_TITKOS];
  char *p = titkos;
  int olvasott_bajtok;
  char kulcs_abc[24]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','p','q','r','s','t','u','v','z','y','w'};
  // titkos fajt berantasa
  while ((olvasott_bajtok =
	  read (0, (void *) p,
		(p - titkos + OLVASAS_BUFFER <
		 MAX_TITKOS) ? OLVASAS_BUFFER : titkos + MAX_TITKOS - p)))
    p += olvasott_bajtok;

  // maradek hely nullazasa a titkos bufferben  
  for (int i = 0; i < MAX_TITKOS - (p - titkos); ++i)
    titkos[p - titkos + i] = '\0';

  // osszes kulcs eloallitasa
  for (int ii = 0; ii <= 23; ++ii)
    for (int ji = 0; ji <= 23; ++ji)
      for (int ki = 0; ki <= 23; ++ki)
	for (int li = 0; li <= 23; ++li)
	  for (int mi = 0; mi <= 23; ++mi)
	 /*   for (int ni = 0; ni <= 23; ++ni)
	      for (int oi = 0; oi <= 23; ++oi)
		for (int pi = 0; pi <= 23; ++pi) */
		  {
		    kulcs[0] = kulcs_abc[ii];
		    kulcs[1] = kulcs_abc[ji];
		    kulcs[2] = kulcs_abc[ki];
		    kulcs[3] = kulcs_abc[li];
		    kulcs[4] = kulcs_abc[mi];
		/*    kulcs[5] = kulcs_abc[ni];
		    kulcs[6] = kulcs_abc[oi];
		    kulcs[7] = kulcs_abc[pi]; */

		    if (exor_tores (kulcs, KULCS_MERET, titkos, p - titkos))
		      printf
			("Kulcs: [%c%c%c%c%c]\nTiszta szoveg: [%s]\n",
		kulcs_abc[ii],kulcs_abc[ji],kulcs_abc[ki],kulcs_abc[li],kulcs_abc[mi], titkos);
			 //ii, ji, ki, li, mi, titkos);

		    // ujra EXOR-ozunk, igy nem kell egy masodik buffer  
		    exor (kulcs, KULCS_MERET, titkos, p - titkos);
		  }

  return 0;
}
