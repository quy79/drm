// main.cc

#define _CRT_SECURE_NO_DEPRECATE
#define CRYPTOPP_DEFAULT_NO_DLL
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1


// Crypto++ Includes
#include <cryptopp/dll.h>
#include <cryptopp/md5.h>
#include <cryptopp/ripemd.h>
#include <cryptopp/rng.h>
#include <cryptopp/gzip.h>
#include <cryptopp/default.h>
#include <cryptopp/randpool.h>
#include <cryptopp/ida.h>
#include <cryptopp/base64.h>
#include <cryptopp/socketft.h>
#include <cryptopp/wait.h>
#include <cryptopp/factory.h>
#include <cryptopp/whrlpool.h>
#include <cryptopp/tiger.h>

// ZipArchive Include
#include <ziparchive/ZipArchive.h>

// Runtime Includes
#include <iostream>
#include <time.h>
#include <iomanip>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <sstream>

#if (_MSC_VER >= 1000)
#include <crtdbg.h>		// for the debug heap
#endif


USING_NAMESPACE(CryptoPP)
USING_NAMESPACE(std)

const int MAX_PHRASE_LENGTH=250;
struct stat results;

using std::ostringstream;
using std::string;
using std::cout;
using std::endl; 

void AES_CTR_Encrypt(const char *hexKey, const char *hexIV, const char *infile, const char *outfile);
SecByteBlock HexDecodeString(const char *hex);


void EncryptFile(const char *in, const char *out, const char *passPhrase);
void DecryptFile(const char *in, const char *out, const char *passPhrase);
/*
BYTE Aes_sBox[] =  {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
             0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
             0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
             0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
             0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
             0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
             0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
             0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
             0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
             0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
             0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
             0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
             0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
             0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
             0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
			 0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};


BYTE Aes_rCon[][4] = { {0x00, 0x00, 0x00, 0x00},
             {0x01, 0x00, 0x00, 0x00},
             {0x02, 0x00, 0x00, 0x00},
             {0x04, 0x00, 0x00, 0x00},
             {0x08, 0x00, 0x00, 0x00},
             {0x10, 0x00, 0x00, 0x00},
             {0x20, 0x00, 0x00, 0x00},
             {0x40, 0x00, 0x00, 0x00},
             {0x80, 0x00, 0x00, 0x00},
             {0x1b, 0x00, 0x00, 0x00},
             {0x36, 0x00, 0x00, 0x00} }; 
*/

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

/*
static bool HexVal(string checkForHex) {
    size_t found;
    found = checkForHex.find_first_not_of("0123456789ABCDEF");
    if (found!=string::npos) {
        // error: This string has non-hex characters in it !
        return false;
    } else {
        // okay passed ! only hex characters.
        return true;
    }
}


static string HexDec(string hexString)
{
    if (HexVal(hexString)) {
        string binaryString;
        StringSource(hexString, true, new HexDecoder(new StringSink(binaryString)));
        return binaryString;
    } else {
        // binary text already! no need to convert to hex
        return hexString;
    }
}


static string HexEnc(string binaryString) {

    if (HexVal(binaryString)) {
        // hex already ! no need to convert binary text
        return binaryString;
    } else {
        string hexString;
        StringSource(binaryString, true, new HexEncoder(new StringSink(hexString)));
        return hexString;
    }
}

static string ByteGen(int howManyBytes) {
    // returns a new random key of the given byte length
    AutoSeededRandomPool rnd;
    byte block[howManyBytes];
    rnd.GenerateBlock(block, howManyBytes);
    string blockString;
    blockString.assign((char *)block, sizeof(block));
    return blockString;
}

static string BitGen(int howManyBits) {
    // returns a new random key in binary of the given bit length
    int byteLength = howManyBits/8; // bitLengths always better be div 8!!
    return ByteGen(byteLength);
}


static string SHA256_Hash(string inputString) {
    // returns a SHA-256 encoded hash of the inputString in binary
    // always returns 256 bits
    SHA256 hash;
    byte digest [ SHA256::DIGESTSIZE ];

    hash.CalculateDigest( digest, (byte *)inputString.c_str(), inputString.length() );
    string hashString;
    hashString.assign((char *)digest, sizeof(digest));

    return hashString;
} 


static void EncryptFileAES(istream& inFile, ostream& outFile, string key, string iv, bool &err, string &errMsg) {
	// will encrypt the file at filenameIn to filenameOut using AES
    // WARNING: The iv must be known and retained for decryption!!
    //			key and iv can be hex or binary

    // convert the key and iv
    key = HexDec(key);
    iv = HexDec(iv);
    try {
		// Set up the encrypter
        CBC_Mode< CryptoPP::AES >::Encryption eas( (byte*)key.c_str(), key.length(), (byte *)iv.c_str() );

        // encrypt		
		FileSource( inFile, true, new StreamTransformationFilter( eas, new FileSink(outFile)));
        err= false;
        return;
	} catch (Exception& e) {
        errMsg = e.GetWhat();
        err = true;
        return;
    }
}


static void DecryptFileAES(istream& inFile, ostream& outFile, string key, string iv, bool &err, string &errMsg) {
    // will encrypt the file at filenameIn to filenameOut using AES
    // WARNING: The correct iv MUST be provided
    // key and iv can be hex or binary

    // convert the key and iv
    key = HexDecode(key);
    iv = HexDecode(iv);

    try {
        // Set up the encrypter
        CBC_Mode< CryptoPP::AES>::Decryption d ( (byte*)key.c_str(), key.length(), (byte *)iv.c_str() );
		FileSource( inFile, true, new StreamTransformationFilter( d, new FileSink(outFile)));
        err= false;
        return;
    } catch (Exception& e) {
		errMsg = e.GetWhat();
		err = true;
		return;
    }
} 
*/

int main(int argc, char *argv[])
{

	string key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4";
	string iv = "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
	const char* keyStr = "N67C9PpD,uqZRG(MxeQWzCdmzqezJGo8tnMk[4s(FpHkdWtY.t";
	string drmExt = ".drm";
	string dirSep = "/";

	try
	{

		/*
		bool err;
        string errMsg;
		AutoSeededRandomPool rng;
		
		string key(AES::DEFAULT_KEYLENGTH, 0);
		string iv(AES::BLOCKSIZE, 0);// Initialization Vector (IV)
		const char* keyStr = "N67C9PpD,uqZRG(MxeQWzCdmzqezJGo8tnMk[4s(FpHkdWtY.t";
		//byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
		//byte iv[ CryptoPP::AES::BLOCKSIZE ];
		

		// digest key
		StringSource(keyStr, true, new HashFilter(*(new SHA256), new ArraySink((unsigned char*)key.c_str(), AES::BLOCKSIZE))); 

		//Create a random IV with default sizes (16)
		rng.GenerateBlock((unsigned char*)iv.c_str(), AES::BLOCKSIZE);
		//rng.GenerateBlock((unsigned char*)key.c_str(), AES::DEFAULT_KEYLENGTH);
		

		//char const *zipFile = "drm.zip";
		//char const *iFile = argv[1];
		//char const *encFile = argv[2];
		//char const *zipFile = argv[3];
		*/
		char const *destDir = argv[1];

		string zipDir = destDir;
		string zipFileName = destDir;
		string zipExt = ".zip";	
		zipFileName.append(zipExt);
		cout << zipFileName << endl;
	

		//get directory file list
		vector<string> files = vector<string>();
		getdir(zipDir,files);

		//prepare zip file
		//CZipArchive zip;
		//zip.Open((char*)zipFileName.c_str(), CZipArchive::zipCreate);    

		for (unsigned int i = 0; i < files.size();i++) {
			string dotStr (".");
			string dotdotStr ("..");
			if ((dotStr.compare(files[i]) != 0) && (dotdotStr.compare(files[i]) != 0)) {




				//cout << files[i] << endl;
				
				char *iFile;
				char *encFile;
				
				size_t found;
				string drmFileName;
				string iFileName;
							
				drmFileName.append(zipDir);
				drmFileName.append(dirSep);
				iFileName.append(zipDir);
				iFileName.append(dirSep);
				
				found = files[i].find_last_of(".");
				drmFileName.append(files[i].substr(0,found));
				drmFileName.append(drmExt);
				iFileName.append(files[i]);
				

				iFile = (char*)iFileName.c_str();
				encFile = (char*)drmFileName.c_str();
				//cout << iFileName << endl;
				
				
				///////////////////////////
				// Now encrypt it using AES method in CTR mode
				int result;
				string cmd = "cryptest.exe ae ";
				string space = " ";
				
				cmd.append(key);
				cmd.append(space);
				cmd.append(iv);
				cmd.append(space);
				cmd.append(iFileName);
				cmd.append(space);
				cmd.append(drmFileName);
				cout << cmd << endl;
				
				//cout << iFileName << endl;
				//cout << drmFileName << endl;
				//result = system((char*)cmd.c_str());
				
				//AES_CTR_Encrypt(key, iv, iFile, encFile);
				//EncryptFile(iFile, encFile, keyStr);	
				// Add ecrypted file to Zip Archive
				/*
				if (stat(encFile, &results) == 0){	
					zip.AddNewFile(encFile, -1, false);
					//remove( iFile );					

				}else{
					// An error occurred
					cout << "0" << endl;
					return -1;
				}
				*/

				///////////////////////////
				
				
				
				//encrypt file and save as .drm	
				//EncryptFile(iFile, encFile, keyStr);



			
			
			}

				
						
					
			
			
		}

		//zip.Close();
		//rmdir(destDir);
		//cout << zipFileName << endl;
		
		//cout << "Passphrase: " << cin.getline(passPhrase, MAX_PHRASE_LENGTH) << endl;
		//cout << "Input file: " << cin.getline(iFile, MAX_PHRASE_LENGTH) << endl;
		//cout << "Output(encrypted) file: " << cin.getline(oFile, MAX_PHRASE_LENGTH) << endl;
		
		//EncryptFile(iFile, encFile, passPhrase);
		//AES_CTR_Encrypt(Aes_sBox, Aes_rCon, iFile, encFile);
		//DecryptFile(encFile, decFile, passPhrase);

		

	}
	catch(CryptoPP::Exception &e)
	{
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return -1;
	}
	catch(std::exception &e)
	{
		cout << "\nstd::exception caught: " << e.what() << endl;
		return -2;
	}
}

void AES_CTR_Encrypt(const char *hexKey, const char *hexIV, const char *infile, const char *outfile)
{
	SecByteBlock key = HexDecodeString(hexKey);
	SecByteBlock iv = HexDecodeString(hexIV);
	CTR_Mode<CryptoPP::AES>::Encryption aes(key, key.size(), iv);
	FileSource(infile, true, new StreamTransformationFilter(aes, new FileSink(outfile)));
}

SecByteBlock HexDecodeString(const char *hex)
{
	StringSource ss(hex, true, new HexDecoder);
	SecByteBlock result((size_t)ss.MaxRetrievable());
	ss.Get(result, result.size());
	return result;
}


void EncryptFile(const char *in, const char *out, const char *passPhrase)
{
	FileSource f(in, true, new DefaultEncryptorWithMAC(passPhrase, new FileSink(out)));	
}

void DecryptFile(const char *in, const char *out, const char *passPhrase)
{
	FileSource f(in, true, new DefaultDecryptorWithMAC(passPhrase, new FileSink(out)));
}

/*
RandomPool & GlobalRNG()
{
	static RandomPool randomPool;
	return randomPool;
}

int (*AdhocTest)(int argc, char *argv[]) = NULL;
*/
