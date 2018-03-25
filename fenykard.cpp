#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int getdir (string dir, vector<string> &files)
{
    //kilistázza a mappák tartalmát, majd berakja azt a vektor végére
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(dir+"/"+string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int main(int argc, char *argv[])
{
    DIR *dp;
    string dir =argv[1];
    string dir2;
    vector<string> files = vector<string>();
    int osszeg=0;
    int osszeg2=0;
    getdir(dir,files);
    bool first=true;
    for (int i = 0;i < files.size();i++) {
	//bejárja azt a vektort, amibe le vannak mentve a mappák nevei
	if(((dp= opendir((files[i]).c_str()))!=NULL)&&(files[i].find("/.")==-1)){
	    //ha az aktuális fájl mappa és nem a "." vagy a "..", akkor arra is meghívja azt a
	    //függvényt, ami kilistázza a tartalmát
	    getdir(files[i],files);
	}else if (files[i].find("/.")==-1){
	    //ha nem mappa, akkor megnyitja a fájlt
	    osszeg2=0;
	    string sor2;
	    string sor;
	    ifstream fin(files[i].c_str());
	    //elkezdi a fájlt soronként beolvasni
	    while(getline(fin,sor)){
		int j=1;
		//ha a sor végén van szóköz, vagy tab akkor letörli őket
		while((sor.substr(sor.size()-1,1)==" ")||(sor.substr(sor.size()-1,1)=="\t")){
		    sor=sor.substr(0,sor.size()-1);
		}
		//ha a sor végén lévő karakter szám, akkor a sor2 változó felveszi azt a karakter
		if((atoi(sor.substr(sor.size()-j,1).c_str()))){
		    sor2=sor.substr(sor.size()-j,1);
		}else {
		    if((sor.substr(sor.size()-1,1)=="0")&&(atoi((sor.substr(sor.size()-2,1)).c_str()))){
			//ha a sor utolsó karakter 0 akkor a sor 2 felveszi az utolsó 2 karaktert
			j+=1;
			sor2=sor.substr(sor.size()-j,2);
		    }
		else
		    //különben a sor2 legyen a betü, hogy majd a következő while-ba be se lépjen
		    sor2="d";
		}
		while((atoi(sor2.c_str()))&&(sor.size()>=j)){
		    //ameddig a sor2 még számmá alakítható, addig mindig legyen egyenlő a sor utolsó
		    //karaktereivel
		    sor2=sor.substr(sor.size()-j,sor.size());
		    j+=1;
		}
		if((sor2!="d")&&(sor.size()>=j)){
		    //ha a sor2 nem d akkor azt jelenti, hogy a sor végén talált számot, és akkor azt
		    //hozzáadhatja az összeg2 változóhoz, ami azt számolja össze, hogy mennyi volt az
		    //összeg fájlonként
		    osszeg2+=atoi((sor2.substr(2,sor2.size())).c_str());
		}
	    }
	    //ha a fájlban lévő pontokat összeadta, akkor azt hozzáadhatja a végleges összeghez
	    osszeg+=osszeg2;
	    //végül bezárja a fájlt és kiírja a fájlonkénti pontot, majd az elérési utat
	    fin.close();
	    cout<<osszeg2<<'\t';
	    cout<<files[i]<<endl;
	}
    }
    cout<<"Az összes megszerezhető pont: "<<osszeg<<endl;
    return 0;
}
