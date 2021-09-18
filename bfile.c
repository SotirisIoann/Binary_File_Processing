#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

struct records{
    char  name[20];
    int bytes;
};

void insert(int *fdata,int *fpos,struct records registrations[200],int *bytes,int *sizee);
void delete(int *fdata,int *fpos,struct records registrations[200],int *sizee);
void display(int *fdata,int *fpos,int sizee);
int search(int *fdata,int *fpos,struct records registrations[200],int sizee,char sur[],int bytes_sur);
void sort(struct records registrations[200],int sizee);
void parse_to_file(int *fdata,int *fpos,struct records registrations[200],int sizee);
void initialization_struct(int *fdata,int *fpos,int *sizee,struct records registrations[200]);

int main(int argc,char* argv[]){
    
    struct records eggrafes[200];
    char x,clear;
    int byte = 0,i;
    int fdata,fpos;
    int sizee = 0;
    int sizefile = 0;
    char n;
    char sur[20];
    int se = 0;
    size_t nbytes2 = 1;
    ssize_t bytes_read;
    ssize_t bytes_sur;

    fdata = open(argv[1],O_RDWR | O_CREAT | O_APPEND,0700);
    if(fdata == -1){
        write(1,"Cannot open data file\n",22);
        return 1;
    }

    fpos = open(argv[2],O_RDWR | O_CREAT | O_APPEND,0700);
    if(fpos == -1){
        write(1,"Cannot open pos file\n",21);
        return 1;
    }

    sizefile = 0;
    do{
        bytes_read = read(fdata, &n, nbytes2);
        sizefile++;
    }while( bytes_read != 0 );

    if(sizefile == 1) write(fpos,&byte,sizeof(byte));
    else{
        initialization_struct(&fdata,&fpos,&sizee,eggrafes);
    }
    
    while(1){
        write(1,"1.Insert\n2.Delete\n3.Search\n4.Display\n0.Exit",44);
        write(1,"\n",1);
        read(0,&x,sizeof(x));
        
        if( x == '0' ) break;
        if( x == '1' ) {
            insert(&fdata,&fpos,eggrafes,&byte,&sizee);
        }
        if( x == '2' ) {
            if(sizee != 0) {
                delete(&fdata,&fpos,eggrafes,&sizee);
            }else{
                read(0, &clear, sizeof(clear));
            }
        }
        if( x == '3' ) {
            if(sizee != 0){
                read(0, &clear, sizeof(clear));
                write(1,"Give the surname that you want to search\n",41);
                bytes_sur = read(0 , sur, sizeof(sur));
                sur[bytes_sur] = ' ';
                se = search(&fdata,&fpos,eggrafes,sizee,sur,bytes_sur);
                if( se != -1 ){ 
                        write(1,eggrafes[se].name,eggrafes[se].bytes);
                        write(1,"\n",1);
                }else{
                    write(1,"This surname that you are looking for doesnt exist\n",52);
                } 
            }else{
                read(0, &clear, sizeof(clear));
            }        
        }
        if( x == '4' ) {
            if(sizee != 0) {
                display(&fdata,&fpos,sizee);
            }else{
                read(0, &clear, sizeof(clear));
            }
        }
        write(1,"\n",1);
    }

    read(0, &clear, sizeof(clear));
    close(fdata);
    close(fpos);
    return 0;
}

void insert(int *fdata,int *fpos,struct records registrations[200],int *bytes,int *sizee){
    char clear;
    read(0, &clear, sizeof(clear));

    write(1,"\n",1);
    write(1,"INSERT\n",7);
    write(1,"\n",1);

    int j;
    char name[20];
    ssize_t bytes_read;

    bytes_read = read(0, name, sizeof(name));
    bytes_read -= 1;

    for(j = 0; j < bytes_read; j++){
        registrations[*sizee].name[j] = name[j];
    }
    registrations[*sizee].bytes = bytes_read;
    *sizee += 1;
    sort(registrations,*sizee);

    parse_to_file(fdata,fpos,registrations,*sizee);
}

void delete(int *fdata,int *fpos,struct records registrations[200],int *sizee){
    write(1,"\n",1);
    write(1,"DELETE\n",7);
    char clear;
    read(0, &clear, sizeof(clear));
    int bytes;
    int x;
    int i,j,temp = 0,thesh = -1;
    char name[20];
    ssize_t bytes_n;

    write(1,"Give the record that you want to delete\n",41);
    bytes_n = read(0 , name, sizeof(name));
    name[bytes_n] = ' ';

    for(i = 0; i < *sizee; i++){
        temp = 0;
        for(j = 0; j < registrations[i].bytes; j++){
            if( name[j] == registrations[i].name[j] ) temp += 1;
        }
        if(temp == registrations[i].bytes) {
            thesh = i;
            break;
        }
    }
    
    if(thesh == -1){
        write(1,"This surname that you are looking for doesnt exist\n",52);
    }else{
        
        for(i = 0; i < registrations[thesh].bytes; i++){
            registrations[thesh].name[i] = ' ';
        }
        registrations[thesh].bytes = 0;
        
        for(i = thesh; i < *sizee; i++){
            for(j = 0; j < registrations[i+1].bytes; j++){
                registrations[i].name[j] = registrations[i+1].name[j];
            }
            registrations[i].bytes = registrations[i+1].bytes;
        }
        
        *sizee -= 1;
        
        parse_to_file(fdata,fpos,registrations,*sizee);
    }
    write(1,"\n",1);
}

int search(int *fdata,int *fpos,struct records registrations[200],int megethos,char sur[],int bytes_sur){
    write(1,"\n",1);
    write(1,"SEARCH\n",7);

    int j,i,y,m;
    int count,z;
    int temp = 0;
    int sizesur[megethos];
    int start = 0;
    int end = megethos;
    int mid,t2;

    y = bytes_sur - 1;
    for (j = 0; j < megethos; j++){
        count = 0;
        for (i = 0; i < registrations[j].bytes; i++){
            count += 1;
            if( registrations[j].name[i] == ' ') break;
        }
        sizesur[j] = count;
    }
    count = 0;
    i = 0;
    j = 0;
    
    while(start <= end){
        
        mid = (start + end)/2;

        z = sizesur[mid];
        if(sur[0] > registrations[mid].name[z]){
            start = mid + 1;
        }else if(sur[0] < registrations[mid].name[z]){
            end = mid - 1;
        }else if(sur[0] == registrations[mid].name[z]){
            temp = 0;
            j = 0;
            z = sizesur[mid];
            m = sizesur[mid];
            while(j < registrations[mid].bytes-m){
                if(sur[j] == registrations[mid].name[z]) {
                    temp += 1;
                }else{
                    break;
                }
                j += 1;
                z += 1;
            }
            if(temp == registrations[mid].bytes-m) {
                return mid;
            }else{
                if(sur[j] > registrations[mid].name[z]){
                    start = mid + 1;
                }else{
                    end = mid - 1;
                }
            }
               
        }
        
    }
    return -1;
}

void display(int *fdata,int *fpos,int sizee){
    write(1,"\n",1);
    write(1,"DISPLAY\n",8);
    write(1,"\n",1);
    
    char clear;
    char x;
    int i,j,k;
    read(0, &clear, sizeof(clear));
    char name[20];
    int size = 0;
    int b[sizee];
    int y;
    ssize_t bytes_read;
    ssize_t bytes_readp;
    size_t nbytes = sizeof(y);

    for(i = 0; i < sizee; i++){
        b[i] = 0;
    }
    lseek(*fpos,0,SEEK_SET);
    i = 0;
    do{
        y = 0;
        bytes_readp = read(*fpos, &y, nbytes);
        b[i] = y;
        size++;
        i++;
    }while( bytes_readp != 0 );
    b[size] = 0;
    size -= 2;
    for( j = 0; j < size; j++ ){
        b[j] = b[j+1] - b[j];    
    }
    lseek(*fdata,0,SEEK_SET);
    i = 0;
    do{
        bytes_read = read(*fdata, name, b[i]); 
        write(1, name, bytes_read);
        write(1,"\n",1);
        i++;
    }while(bytes_read != 0);
}


void sort(struct records registrations[200],int sizee){
    int i,j,k,x,s1,s2,k1;
    char temp[20];
    int s = 0,t = 0,t2 = 0;
    int count;
    int sizesur[sizee];

    for (j = 0; j < sizee; j++){
        count = 0;
        for (i = 0; i < registrations[j].bytes; i++){
            count += 1;
            if( registrations[j].name[i] == ' ') break;
        }
        sizesur[j] = count;
    }

    for (i = 0; i < sizee-1; i++) {
        for (j = i+1; j < sizee; j++) {
            s1 = registrations[i].bytes;
            s2 = registrations[j].bytes;

            k1 =  sizesur[j];
            for(k = sizesur[i]; k < 20; k++){

                if( registrations[i].name[k] > registrations[j].name[k1]){
                    for(x = 0 ; x < s1; x++){
                        temp[x] = registrations[i].name[x];
                    }
                    t = registrations[i].bytes;
                    t2 = sizesur[i];
                    for(x = 0 ; x < s2; x++){
                        registrations[i].name[x] = registrations[j].name[x];
                    }
                    registrations[i].bytes = registrations[j].bytes;
                    sizesur[i] = sizesur[j];
                    for(x = 0 ; x < t; x++){
                        registrations[j].name[x] = temp[x];
                    }
                    registrations[j].bytes = t;
                    sizesur[j] = t2;
                    break;
                }
                if( registrations[i].name[k] < registrations[j].name[k1]) break;
                k1 += 1;
            }
        }
    } 
}

void parse_to_file(int *fdata,int *fpos,struct records registrations[200],int sizee){
    int i;
    int byte;
    
    ftruncate(*fdata,0);
    ftruncate(*fpos,0);
    lseek(*fdata,0,SEEK_SET);
    lseek(*fpos,0,SEEK_SET);

    byte = 0;
    write(*fpos,&byte,4);

    lseek(*fdata,0,SEEK_END);
    lseek(*fpos,0,SEEK_END);

    for(i = 0; i < sizee; i++ ){   
        write(*fdata, &registrations[i].name, registrations[i].bytes);
        byte += registrations[i].bytes;
        write(*fpos, &byte, 4);
    }
}

void initialization_struct(int *fdata,int *fpos,int *sizee,struct records registrations[200]){
    ssize_t bytes_readp;
    int y,i;
    int size = 0;
    int j;
    int b[200];
    char name[20];
    size_t nbytes = sizeof(y);
    ssize_t bytes_read;

    for(i = 0; i < 200; i++){
        b[i] = 0;
    }
    
    lseek(*fpos,0,SEEK_SET);
    i = 0;
    do{
        y = 0;
        bytes_readp = read(*fpos, &y, nbytes);
        b[i] = y;
        size++;
        i++;
    }while( bytes_readp != 0 );

    b[size] = 0;
    size -= 2;
    for( j = 0; j < size; j++ ){
        b[j] = b[j+1] - b[j];
        registrations[j].bytes = b[j];    
    }
    
    lseek(*fdata,0,SEEK_SET);
    i = 0;
    do{
        bytes_read = read(*fdata, name, b[i]); 
        for( j = 0; j < b[i]; j++ ){
            registrations[i].name[j] = name[j];
        }
        i++;
    }while(bytes_read != 0);
    *sizee = i - 1;
}