#include <stdio.h>   //header files
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

OperationType check_operation_type(char *argv[]);  //checking operation type

int main( int argc,char *argv[] )  //main function
{
    EncodeInfo EncInfo;            //structure variable declartaion
    DecodeInfo DecInfo;

    if( argc < 2 )                   //validating cla
    {
	printf("Insufficient no.of arguments\n");
	return 0;
    }


    if( check_operation_type(argv) == e_encode )         //checking encode or not
    {
	if( argc >=4 )                               //validating cla
	{
    	    printf("\nSELECTED ENCODING\n\n");
    	    Do_Encoding(argv,&EncInfo);          //doing encoding
	}
	else
	{
	    printf("4 or More Arguments is mandatory for ENCODING\n");
	    return 0;
    	}
    }

    else if( check_operation_type(argv) == e_decode )  //checking decode or not
    {
	if( argc >=3 )                            //validating cla
	{
    	    printf("\nSELECTED DECODING\n\n");
    	    Do_Decoding(argv,&DecInfo);           //doing decoding
    	}
	else
	{
	    printf("3 or More Arguments is mandatory for DECODING\n");
	    return 0;
	}
    }
    else
    {
	printf("INVALID CHOICE!\n");
    }

    
    return 0;
}

OperationType check_operation_type(char *argv[])  //checking encode/decode
{
    if( (strcmp(argv[1],"e"))  == 0 )
        return e_encode;
    else if( (strcmp(argv[1],"d"))  == 0 )
        return e_decode;
    else
        return  e_unsupported;
}


Status Do_Encoding( char *argv[],EncodeInfo *EncInfo)         //start encoding
{
  if( read_and_validate_encode_args(argv,EncInfo) == e_success ) //validating arguments
  {
      printf("4) Read and Validate SUCCESS\n");
  }
  else
  {
      printf("Read and Validate FAILURE\n");
      return e_failure;
  }

  if( open_files(EncInfo) == e_success )   //opening files
  {
      printf("5) Open files SUCCESS\n");
  }
  else
  {
      printf("Open files FAILURE\n");
      return e_failure;
  }

  if( check_capacity(EncInfo) == e_success ) //checking capacity 
  {
      printf("6) Check capacity SUCCESS\n");
  }
  else
  {
      printf("Check Capacity FAILURE\n");
      return e_failure;
  }

  if( copy_bmp_header( EncInfo->Src_Img_Pointer,EncInfo->Dest_Img_Pointer ) == e_success ) //to copy bmp header
  {
      printf("7) Copy Bmp header is SUCCESS\n\n");
  }
  else
  {    
      printf("Copy Bmp header is FAILURE\n");
      return e_failure;
  }

  if( encode_magic_string(MAGIC_STRING,EncInfo) == e_success )  //to encode magic string
  {
      printf("8)  (%s)   Encoding Magic String is SUCCESS\n",MAGIC_STRING);
  }
  else
  {
      printf("Encoding Magic String is FAILURE\n");
      return e_failure;
  }

  if( encode_secret_file_extn_size(strlen(EncInfo->Scrt_File_Extn),EncInfo->Src_Img_Pointer,EncInfo->Dest_Img_Pointer) == e_success ) //to encode secret file extension size
  {
      printf("9)  (%ld)    Encoding Secret file extension size is SUCCESS\n",strlen(EncInfo->Scrt_File_Extn));
  }
  else
  {
      printf("Encoding Secret file extension is size FAILURE\n");
      return e_failure;
  }
  if( encode_secret_file_extn(EncInfo->Scrt_File_Extn,EncInfo) == e_success )  //to encode secret file extension
  {
      printf("10) (%s) Encoding Secret file extension is SUCCESS\n",EncInfo->Scrt_File_Extn);
  }
  else
  {
      printf("Encoding Secret file extension is FAILURE\n");
      return e_failure;
  }
  if( encode_secret_file_size(EncInfo->Scrt_File_Size,EncInfo) == e_success ) //to encode secret file
  {
      printf("11) (%d)    Encoding Secret file size is SUCCESS\n\n",EncInfo->Scrt_File_Size-1);
  }
  else
  {
      printf("Encoding Secret file extension is FAILURE\n");
      return e_failure;
  }
  if( encode_secret_file_data(EncInfo) == e_success ) //to encode secret file data
  {
      printf("12) Encoding secret file data is SUCCESS\n");
  }
  else
  {
      printf("Encoding secrt file data is FAILURE\n");
      return e_failure;
  }
  if( copy_remaining_img_data(EncInfo->Src_Img_Pointer,EncInfo->Dest_Img_Pointer) == e_success ) //to copy the rest of the image data
  {
      printf("13) Copy Remaining data is SUCCESS\n\n");
  }
  else
  {
      printf("Copy Remaining data is FAILURE\n");
      return e_failure;
  }

  return e_success;
}


Status Do_Decoding( char *argv[],DecodeInfo *DecInfo)  //performing decoding
{
    if( Read_And_Validate_Decode_Args(argv,DecInfo) == e_success ) //validationg cla
    {
	printf("2) Read and Validate SUCCESS\n");
    }
    else
    {
        printf("Read and Validate FAILURE\n");
        return e_failure;
    }

    if( Open_Files( DecInfo )  == e_success )  //opening files
    {
        printf("3) Open files SUCCESS\n\n");
    }
    else
    {
	 printf("Open files FAILURE\n");
	 return e_failure;
    }

    if( Decode_Magic_String(DecInfo) == e_success ) //decoding magic string
    {
	printf("Decode Magic String is SUCCESS\n5)");
    }
    else
    {
	printf("Decode Magic String is FAILURE\n");
	return e_failure;
    }

    if( Decode_Scrt_File_Extn_Size( &DecInfo->Scrt_File_Extn_Size,DecInfo) == e_success ) //decoding secret file extn size
    {
	printf("Decode Secret File Extension Size is SUCCESS\n");
    }
    else
    {
	printf("Decode Secret File Extension Size is FAILURE\n");
        return e_failure;
    }

    if( Decode_Scrt_File_Extn(DecInfo) == e_success )   //decoding secret file extension
    {
	 printf(" Decode Secret File Extension is SUCCESS\n7)");
    }
    else
    {
	 printf("Decode Secret File Extension is FAILURE\n");
	 return e_failure;
    }

    if( Decode_Scrt_File_Size(DecInfo) == e_success )  //decoding secret file size
    {
	printf("Decode Secret File Size SUCCESS\n");
    }
    else
    {
	printf("Decode Secret File Size FAILURE\n");
    }

    if(  Decode_Scrt_File_Data(DecInfo) == e_success )  //decoding secret file data
    {
	printf("9) Decode Secret File Data is SUCCESS\n");
    }
    else
    {
	 printf("Decode Secret File Data is FAILURE\n");
    }

    return e_success;
}



