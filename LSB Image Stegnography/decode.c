#include<stdio.h>    //header files
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include<stdlib.h>

Status Read_And_Validate_Decode_Args( char *argv[],DecodeInfo *DecInfo) //to validate cla
{
    if(strstr(argv[2],".") == NULL)    //to validate bmp file
    { 
	printf("No extension in input file\n");
	return e_failure;
    }
    else if( strcmp( strstr(argv[2],"."),".bmp" ) == 0 )
    {
        printf("1) Bmp File is Present\n");
        DecInfo->Src_Img_Name = argv[2];
    }
    else if( strcmp( strstr(argv[2],"."),".bmp" ) != 0 )
    {
        DecInfo->Src_Img_Name = argv[2];
    }
    else
    {
        printf("ERROR:In Bmp file\n");
        return e_failure;
    }
    

    if( argv[3] == NULL )         //to validate output file
    {
     strcpy(DecInfo->Output_File_Name,"Output");
    }
    else
    {
        strcpy(DecInfo->Output_File_Name,argv[3]);
    }
    
    return e_success;
}

Status Open_Files(DecodeInfo *DecInfo) //to open files
{
    DecInfo->Src_Img_Pointer = fopen(DecInfo->Src_Img_Name,"r");

    if( DecInfo->Src_Img_Pointer == NULL )
    {
      perror("fopen");
      fprintf(stderr,"ERROR : Unable to open file %s\n",DecInfo->Src_Img_Name);

      return e_failure;
    }
    return e_success;
}

Status Decode_Magic_String(DecodeInfo *DecInfo)  //to decode the magic string
{
  char *Magicdata=malloc(2);
  fseek(DecInfo->Src_Img_Pointer,54,SEEK_SET);
  Decode_Image_To_Data(Magicdata,2,DecInfo);
  if( strcmp(Magicdata,MAGIC_STRING) == 0 )
  {
      printf("4) (%s)   ",Magicdata);
      return e_success;
  }
  else
  {
      return e_failure;
  }
}

Status Decode_Image_To_Data ( char *Data ,int size , DecodeInfo *DecInfo) //to decode the image to data
{ 
    int i;
    for(i=0;i<size;i++)
    {
        fread(DecInfo->Image_Data,8,1,DecInfo->Src_Img_Pointer);
        Decode_Byte_From_Lsb(&Data[i],DecInfo->Image_Data);
    }

    return e_success;
}

Status Decode_Byte_From_Lsb(char *data, char* Image_Data) //to decode byte from lsb
{
   int i;
   *data=0;
   for(i=0;i<8;i++)
   {
       *data = ((Image_Data[i] & 1))<<i | *data ;
   }
   return e_success;
}

Status Decode_Scrt_File_Extn_Size( int *Sizedata ,DecodeInfo *DecInfo ) //to decode secret file extension size
{
    int size=0,i;
    fread(DecInfo->Image_Data,32,1,DecInfo->Src_Img_Pointer); //redaing image data

    for(i=0 ; i<32 ; i++ )
    {
	 size = ((DecInfo->Image_Data[i] & 1))<<i | size;
    }
    *Sizedata = size;
    printf(" (%d)    ",*Sizedata);
    return e_success;
}

Status Decode_Scrt_File_Extn(DecodeInfo *DecInfo) //to decode secret file extension
{
    DecInfo->Scrt_File_Extn=malloc(4);
   if( Decode_Image_To_Data( DecInfo->Scrt_File_Extn,DecInfo->Scrt_File_Extn_Size,DecInfo) == e_success)
   {
    printf("6) (%s) ",DecInfo->Scrt_File_Extn);
    return e_success;
   }
}

Status Decode_Scrt_File_Size(DecodeInfo *DecInfo)  //to decode secret file size
{
     if(Decode_Scrt_File_Extn_Size( &DecInfo->Scrt_File_Size,DecInfo) == e_success)
     {
	 return e_success;
     }
}

Status Decode_Scrt_File_Data(DecodeInfo *DecInfo)   //to decode secret file data
{ 
    char *string=malloc(DecInfo->Scrt_File_Size); //dynamic memmory allocation
    int i;

    DecInfo->Scrt_File_Extn[DecInfo->Scrt_File_Extn_Size+1] = '\0';
   
    strcat(DecInfo->Output_File_Name,DecInfo->Scrt_File_Extn);  //concatenating extension
    Decode_Image_To_Data(string,DecInfo->Scrt_File_Size,DecInfo);
    
    printf("\n8) Secret = %s\n",string);
    DecInfo->Scrt_File_Pointer = fopen(DecInfo->Output_File_Name,"w"); //opening the file
    fwrite(string,DecInfo->Scrt_File_Size,1,DecInfo->Scrt_File_Pointer); //writing into file
   
    return e_success;
}




	   
     
    
	


    
