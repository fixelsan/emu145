const unsigned int ik1302_urom[]={
0x0000000, //
0x0800001, //sum=R[i]; S=sum; 
0x0A00820, //sum=(S)+(0x1); S=sum; L=cry; 
0x0040020, //sum=S; R[i-1]=sum; 
0x0A03180, //sum=(~S|S)+(L|!L); S=sum; L=cry; 
0x0203081, //sum=(R[i])+(S)+(L|!L); L=cry; 
0x0A00181, //sum=(R[i])+(~S|S); S=sum; L=cry; 
0x0803800, //sum=(0x1)+(L|!L); S=sum; 
0x0818001, //sum=R[i]; R[i]=S; S=sum; 
0x0800400, //sum=(0x6); S=sum; 
0x0A00089, //sum=(R[i]|~R[i])+(S); S=sum; L=cry; 
0x0A03C20, //sum=(S)+(0x1|0x6)+(L|!L); S=sum; L=cry; 
0x0800820, //sum=(S)+(0x1); S=sum; 
0x0080020, //sum=S; R[i-2]=sum; 
0x0800180, //sum=(~S|S); S=sum; 
0x1400020, //sum=S; S=S1; S1=sum; 
0x0800081, //sum=(R[i])+(S); S=sum; 
0x0210801, //sum=(R[i])+(0x1); R[i]=sum; L=cry; 
0x0043180, //sum=(~S|S)+(L|!L); R[i-1]=sum; 
0x0058001, //sum=R[i]; R[i]=S; R[i-1]=sum; 
0x0808001, //sum=R[i]; R[i]=R[i+3]; S=sum; 
0x0A03081, //sum=(R[i])+(S)+(L|!L); S=sum; L=cry; 
0x0A01081, //sum=(R[i])+(S)+(L); S=sum; L=cry; 
0x0A01181, //sum=(R[i])+(~S|S)+(L); S=sum; L=cry; 
0x0040090, //sum=(0xA*!L)+(S); R[i-1]=sum; 
0x0800401, //sum=(R[i])+(0x6); S=sum; 
0x0A00081, //sum=(R[i])+(S); S=sum; L=cry; 
0x0040001, //sum=R[i]; R[i-1]=sum; 
0x0800801, //sum=(R[i])+(0x1); S=sum; 
0x1003180, //sum=(~S|S)+(L|!L); S1=sum; 
0x0800100, //sum=(~S); S=sum; 
0x1200801, //sum=(R[i])+(0x1); L=cry; S1=sum; 
0x0013C01, //sum=(R[i])+(0x1|0x6)+(L|!L); R[i]=sum; 
0x0800008, //sum=~R[i]; S=sum; 
0x0A00088, //sum=(~R[i])+(S); S=sum; L=cry; 
0x0010200, //sum=(S1); R[i]=sum; 
0x0800040, //sum=0x4; S=sum; 
0x0801300, //sum=()+(~S|S1)+(L); S=sum; //0x0801280, //sum=(S|S1)+(L); S=sum;
0x1801200, //sum=(S1)+(L); S=sum; S1=sum; 
0x1000208, //sum=(~R[i])+(S1); S1=sum; 
0x0080001, //sum=R[i]; R[i-2]=sum; 
0x0A00082, //sum=(M[i])+(S); S=sum; L=cry; 
0x0A01008, //sum=~R[i]+(L); S=sum; L=cry; 
0x1000001, //sum=R[i]; S1=sum; 
0x0A00808, //sum=(~R[i])+(0x1); S=sum; L=cry; 
0x0900001, //sum=R[i]; M[i]=S; S=sum; 
0x0010004, //sum=ST[i]; R[i]=sum; 
0x0080820, //sum=(S)+(0x1); R[i-2]=sum; 
0x0800002, //sum=M[i]; S=sum; 
0x0140002, //sum=M[i]; R[i-1]=sum; M[i]=S; 
0x0008000, //R[i]=R[i+3]; 
0x0A00090, //sum=(0xA*!L)+(S); S=sum; L=cry; 
0x0A00220, //sum=(S)+(S1); S=sum; L=cry; 
0x0801001, //sum=R[i]+(L); S=sum; 
0x1203200, //sum=(S1)+(L|!L); L=cry; S1=sum; 
0x4800001, //sum=R[i]; S=sum; ST[i]=sum;
0x0011801, //sum=(R[i])+(0x1)+(L); R[i]=sum; 
0x1808001, //sum=R[i]; R[i]=R[i+3]; S=sum; S1=sum; 
0x0A04020, //sum=S+(!T); S=sum; L=cry; 
0x4800801, //sum=(R[i])+(0x1); S=sum; ST[i]=sum;
0x0840801, //sum=(R[i])+(0x1); R[i-1]=sum; S=sum; 
0x0840020, //sum=S; R[i-1]=sum; S=sum; 
0x0813081, //sum=(R[i])+(S)+(L|!L); R[i]=sum; S=sum; 
0x0810801, //sum=(R[i])+(0x1); R[i]=sum; S=sum; 
0x0810180, //sum=(~S|S); R[i]=sum; S=sum; 
0x0800180, //sum=(~S|S); S=sum; 
0x0A00081, //sum=(R[i])+(S); S=sum; L=cry; 
0x0800001, //sum=R[i]; S=sum; 
};

