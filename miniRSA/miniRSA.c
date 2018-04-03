/*
 * @file    rsa.c
 * @author  작성자 이름 / 학번
 * @date    작성 일자
 * @brief   mini RSA implementation code
 * @details 세부 설명
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miniRSA.h"

uint p, q, e, d, n;

/*
 * @brief     모듈러 덧셈 연산을 하는 함수.
 * @param     uint a     : 피연산자1.
 * @param     uint b     : 피연산자2.
 * @param     byte op    : +, - 연산자.
 * @param     uint n      : 모듈러 값.
 * @return    uint result : 피연산자의 덧셈에 대한 모듈러 연산 값. (a op b) mod n
 * @todo      모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */

uint GCD(uint a, uint b);


uint ModAdd(uint a, uint b, byte op, uint n) {

   uint result = 0;

    if(op == '-') {
        if(a >= b) {
            result = a - b;
        }
        else {
            result = n - (b - a);
        }
    }
    else if(op == '+') {
        if(((a + b) < a) || ((a + b) < b)) {
            result = a - (n - b);
        }
        else if((a + b) >= n) {
            result = (a + b) - n;
        }
        else {
            result = (a + b);
        }
    }
    return result;
}

/*
 * @brief      모듈러 곱셈 연산을 하는 함수.
 * @param      uint x       : 피연산자1.
 * @param      uint y       : 피연산자2.
 * @param      uint n       : 모듈러 값.
 * @return     uint result  : 피연산자의 곱셈에 대한 모듈러 연산 값. (a x b) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
uint ModMul(uint x, uint y, uint n) {
   

    uint tmp = x;
    uint result = 0;

    while(y > 0) {     
        if((y & 1) == 1) { // 홀수인지 짝수인지 엔드값이 1 이면 홀수 
            result = ModAdd(result,tmp,'+', n); // result랑 temp를 더한 모듈러를 result에 저장 
        }
        tmp = ModAdd(tmp,tmp, '+', n);// temp를 두번 더하고 n으로 모듈러 한걸 temp에 저장 
        y >>= 1; //오른쪽으로 한칸 쉬프트 , 나중엔 0이됨 
    }
    
    return result;
}

/*
 * @brief      모듈러 거듭제곱 연산을 하는 함수.
 * @param      uint base   : 피연산자1.
 * @param      uint exp    : 피연산자2.
 * @param      uint n      : 모듈러 값.
 * @return     uint result : 피연산자의 연산에 대한 모듈러 연산 값. (base ^ exp) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
               'square and multiply' 알고리즘을 사용하여 작성한다.
 */
uint ModPow(uint base, uint exp, uint n) {

   uint result = 1; //지수가 0이면 1 출력 , 

    while(exp > 0) { // 지수가 0보다 크면 
        if((exp & 1) == 1) {
            result = ModMul(result,base,n);
        }
        base = ModMul(base,base,n);
        exp >>= 1; //쉬프트 하다보면 0이됨
    }

    return result; 
}

/*
 * @brief      입력된 수가 소수인지 입력된 횟수만큼 반복하여 검증하는 함수.
 * @param      uint testNum   : 임의 생성된 홀수.
 * @param      uint repeat    : 판단함수의 반복횟수.
 * @return     uint result    : 판단 결과에 따른 TRUE, FALSE 값.
 * @todo       Miller-Rabin 소수 판별법과 같은 확률적인 방법을 사용하여,
               이론적으로 4N(99.99%) 이상 되는 값을 선택하도록 한다. 
 */

uint Division(uint a, uint b){
    uint cnt = 0;
    while (a >= b)
    {
        a -= b;
        cnt++;
    }
    return cnt;
}



bool IsPrime(uint testNum, uint repeat) {// ( 1 - 1/(4^repeat) )의 확률로 소수라고 할 수 있다. repeat가 7일때  99.9975확률로 


    uint a, b;
    uint m; 
    int k=0;
    int i, j;
   
 
    // n-1 = (2^k)*m 을 만족하는 m, k를 구한다. 
    m= testNum-1;
    while(ModPow(m,1,2)==0) //m이 홀수가 되면 그만둔다. 
    {
      m=Division(m,2);
      k++;      
    }
   

    for( i=0; i<repeat; i++ )
    {
        
        a = WELLRNG512a()*100; //임의의수 a 설정 

        if( GCD( a, testNum ) != 1 ) //GCD가 1이면 합성수다
        {
            return FALSE;
        }

        // b = a^m mod n 연산을 수행한다.
        b = ModPow(a,m,testNum);
 
        if( b == 1 || b == (testNum-1) )
        {
            continue;       
        }
        else
        {
            for( j=0; j<k-1; j++ )
            {
                b = ModMul(b, b, testNum);
                if( b == (testNum-1) )       
                    return TRUE;
                    // break;
            }
            // 소수가 아닌 경우
            if( b != (testNum-1) )
            {
                return FALSE;
            }
        }
    }
    return TRUE;
   
}

/*
 * @brief       모듈러 역 값을 계산하는 함수.
 * @param       uint a      : 피연산자1.
 * @param       uint m      : 모듈러 값.
 * @return      uint result : 피연산자의 모듈러 역수 값.
 * @todo        확장 유클리드 알고리즘을 사용하여 작성하도록 한다.
 */
uint Mod(uint a, uint b){
    while (a >= b) a -= b;
    return a;
}


uint ModInv(uint a, uint m) { //확장 유클리드 알고리즘 사용 

  uint r, q, s, s1=1, s2=0, t, t1=0, t2=1, tmp = a;

  while(m)
  {
   q = Division(a,m);
   r = Mod(a,m);
   s = s1 - q*s2;
   t = t1 - q*t2;


   a = m;
   m = r;
   s1 = s2; 
   s2 = s;
   t1 = t2;
   t2 = t;
  }
  

  if(a == 1) //역원이 있음
  {
   
   return t1;
  }

  return 0;
}

uint mul(uint n, uint m){
    int result = 0;
    int a= 1;

    while(n!=a){

        result= result+m;
        a++;

        if(result>(result+m)){
                   return 1;
        }   

    }

        return result;
}



/*
 * @brief     RSA 키를 생성하는 함수.
 * @param     uint *p   : 소수 p.
 * @param     uint *q   : 소수 q.
 * @param     uint *e   : 공개키 값.
 * @param     uint *d   : 개인키 값.
 * @param     uint *n   : 모듈러 n 값.
 * @return    void
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */


void miniRSAKeygen(uint *p, uint *q, uint *e, uint *d, uint *n) {

    uint prime = WELLRNG512a()*100000;
    uint prime2 = WELLRNG512a()*100000;


    while(IsPrime(prime,7)==0 || IsPrime(prime2,7)==0 || (prime*prime2)<2147483647 || mul(prime,prime2) ==1){  
           prime = WELLRNG512a()*100000;
           prime2 = WELLRNG512a()*100000;
          
    }

    *n= prime * prime2;

    printf("생성한 난수 p:%d\n",prime);
    printf("생성한 난수 q:%d\n",prime2);
    
    *p= prime;
    *q= prime2;
    

    uint pi = (prime-1)*(prime2-1) ;    
    
    *e=WELLRNG512a()*1000000; // e 는 적당히 커야 ModPow가 가능하다. 


    // while(IsPrime(*e,4) ==0){
    while(GCD(*e,pi)!=1||pi<=*e||ModMul(*e,*d,pi)!=1) {
            *e=*e+1;
            *d=ModInv(pi,*e);

            printf("진행중s\n");
    }
    if(GCD(*e,pi)!=1){ //예외 처리
        perror("e값을 잘못 생성하였습니다.\n");
        exit(1);
    }
    if(ModMul(*e,*d,pi)!=1){ //예외 처리
        perror("d값을 잘못 생성하였습니다.\n");
        exit(1);
    }
    
    printf("e =%u\n",*e );
    printf("d =%u\n",*d );
    printf("pi : %u\n",pi);

    printf("p * q = n값은 %u 입니다. \n", (*p * *q));
    if(*n!=(*p * *q)){ //예외 처리
        perror("n값을 잘못 생성하였습니다.\n");
        exit(1);
    }

    printf("(e*d) mod pi 값은 %u 입니다. \n\n", ModMul(*e,*d,pi) );

    printf("곱셈값은 %u\n",mul(*p,*q));

}

/*
 * @brief     RSA 암복호화를 진행하는 함수.
 * @param     uint data   : 키 값.
 * @param     uint key    : 키 값.
 * @param     uint n      : 모듈러 n 값.
 * @return    uint result : 암복호화에 결과값
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */
uint miniRSA(uint data, uint key, uint n) {


    if(data>=n||data<1){
        perror("n이 작아 암호화 할수없습니다.");
        exit(1);
    }
    printf("input data : %u\n",data );
    uint result;

    result= ModPow(data,key,n);
    printf("output data : %u\n",result);


    return result;
}

uint GCD(uint a, uint b) {
    uint prev_a;

    while(b != 0) {
        printf("GCD(%u, %u)\n", a, b);
        prev_a = a;
        a = b;
        while(prev_a >= b) prev_a -= b;
        b = prev_a;
    }
    printf("GCD(%u, %u)\n\n", a, b);
    return a;
}


int main(int argc, char* argv[]) {
    byte plain_text[4] = {0x12, 0x34, 0x56, 0x78};
    uint plain_data, encrpyted_data, decrpyted_data;
    uint seed = time(NULL);

    memcpy(&plain_data, plain_text, 4);

    // 난수 생성기 시드값 설정
    seed = time(NULL);
    InitWELLRNG512a(&seed);

    // RSA 키 생성
    miniRSAKeygen(&p, &q, &e, &d, &n);
    printf("0. Key generation is Success!\n ");
    printf("p : %u\n q : %u\n e : %u\n d : %u\n N : %u\n\n", p, q, e, d, n);

    // RSA 암호화 테스트

    encrpyted_data = miniRSA(plain_data, e, n);
    printf("1. plain text : %u\n", plain_data);    
    printf("2. encrypted plain text : %u\n\n", encrpyted_data);

    // RSA 복호화 테스트
    decrpyted_data = miniRSA(encrpyted_data, d, n);
    printf("3. cipher text : %u\n", encrpyted_data);
    printf("4. Decrypted plain text : %u\n\n", decrpyted_data);

    // 결과 출력
    printf("RSA Decryption: %s\n", (decrpyted_data == plain_data) ? "SUCCESS!" : "FAILURE!");

    return 0;
}