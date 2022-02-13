{
    /*This file is a sample*/
    int n;
    int in;
    int sum;
    int i;

    sum=100;

    read n;
    for(i=0;i<n;i=i+1){
        read in;
        sum = sum + in;
    }
    write sum;
}