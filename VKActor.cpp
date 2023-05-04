// Fill out your copyright notice in the Description page of Project Settings.


#include "VKActor.h"
#include "Windows/AllowWindowsPlatformTypes.h"

// Sets default values
AVKActor::AVKActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVKActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVKActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVKActor::ReleasedShift()
{
	keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void AVKActor::PressedShift()
{
	keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
}

void AVKActor::SendKeyInput(EVirtualKeyBoardState KeyLangState, FString LastString, int inputkeycode)
{
	KeyCode = inputkeycode;

	switch (KeyLangState)
	{
		case EVirtualKeyBoardState::EN_Lower:
		{
			ReleasedShift();
			SetCurrentInputMode(false);
			keybd_event(KeyCode, 0, 0, 0);

			break;
		}
		case EVirtualKeyBoardState::EN_Upper:
		{
			PressedShift();
			SetCurrentInputMode(false);

			keybd_event(KeyCode, 0, 0, 0);
			break;
		}
		case EVirtualKeyBoardState::KR_Lower:
		{
			ReleasedShift();
			SetCurrentInputMode(true);

			//이전 입력 문자가 있고, 한글이라면
			if (LastString.Len() > 0 && SubstringLastWord(LastString).GetCharArray()[0] > 255)
			{
				
				int LastStringLen = LastString.Len();

				TCHAR* Str = &LastString.GetCharArray()[LastStringLen-1];


				TCHAR Buffer[4096];			
				BreakStr(Str, Buffer, sizeof(Buffer));				
				//FString Result = FString(Buffer);			
				FString Result = FString(Buffer);
				//이전문자 삭제												
				BackSpaceFunc();

				

				//키보드 입력 반복처리
				for(int i = 0; i<Result.GetCharArray().Num()-1; i++)
				{
					keybd_event(VkKeyScan(Result.GetCharArray()[i]), 0, KEYEVENTF_EXTENDEDKEY, 0);
					keybd_event(VkKeyScan(Result.GetCharArray()[i]), 0, KEYEVENTF_KEYUP, 0);
				}
				keybd_event(KeyCode, 0, KEYEVENTF_EXTENDEDKEY, 0);
				keybd_event(KeyCode, 0, KEYEVENTF_KEYUP, 0);
	
			}

			//이전 입력 문자가 없더라면
			else
			{
				keybd_event(KeyCode, 0, 0, 0);
			}

			break;
		}
		case EVirtualKeyBoardState::KR_Upper:
		{
			PressedShift();
			SetCurrentInputMode(true);
			//이전 입력 문자가 있고, 한글이라면
			if (LastString.Len() > 0 && SubstringLastWord(LastString).GetCharArray()[0] > 255)
			{

				int LastStringLen = LastString.Len();

				TCHAR* Str = &LastString.GetCharArray()[LastStringLen - 1];


				TCHAR Buffer[4096];
				BreakStr(Str, Buffer, sizeof(Buffer));
				//FString Result = FString(Buffer);			
				FString Result = FString(Buffer);
				//이전문자 삭제												
				BackSpaceFunc();



				//키보드 입력 반복처리
				for (int i = 0; i < Result.GetCharArray().Num() - 1; i++)
				{
					keybd_event(VkKeyScan(Result.GetCharArray()[i]), 0, KEYEVENTF_EXTENDEDKEY, 0);
					keybd_event(VkKeyScan(Result.GetCharArray()[i]), 0, KEYEVENTF_KEYUP, 0);
				}
				keybd_event(KeyCode, 0, KEYEVENTF_EXTENDEDKEY, 0);
				keybd_event(KeyCode, 0, KEYEVENTF_KEYUP, 0);

			}

			//이전 입력 문자가 없더라면
			else
			{
				keybd_event(KeyCode, 0, 0, 0);
			}
			break;
		}
	}
}

void AVKActor::SetCurrentInputMode(bool IsKR)
{
	DWORD conv, sentence;

	//IME 핸들을 얻는다
	HIMC h_mic = ImmGetContext(GetActiveWindow());

	//IME 상태를 얻는다
	ImmGetConversionStatus(h_mic, &conv, &sentence);

	if (IsKR)
	{
		conv = conv & (~IME_CMODE_CHARCODE);
		conv = conv | IME_CMODE_LANGUAGE;
	}
	else
	{
		conv = conv & (~IME_CMODE_LANGUAGE);
		conv = conv | IME_CMODE_CHARCODE;
	}

	ImmSetConversionStatus(h_mic, conv, sentence);
	ImmReleaseContext(GetActiveWindow(), h_mic);
}

FString AVKActor::SubstringLastWord(FString InString)
{
	return InString.Len() > 0 ? InString.Right(1) : FString();
}

int AVKActor::BreakStr(TCHAR* str, TCHAR* buffer, UINT nSize)
{
	//한글을 초성,중성,종성 구분을 해주고 분해한 수만큼 버퍼가 생성됨

	//초성
	TArray<TCHAR>KrHead = { L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };
	TArray<TCHAR>EnHead = { L'r',L'R',L's', L'e', L'E', L'f', L'a', L'q', L'Q', L't', L'T',L'd',L'w',L'W',L'c',L'z',L'x',L'v',L'g' };
	//중성
	TArray<TCHAR>KrBody = { L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ', L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ' };
	TArray<TCHAR>EnBody1 = { L'k',L'o', L'i', L'O', L'j', L'p',L'u',L'P',L'h',L'h',L'h',L'h',L'y',L'n',L'n',L'n',L'n',L'b',L'm',L'm',L'l' };
	TArray<TCHAR>EnBody2 = { L'k',L'o', L'i', L'O', L'j', L'p',L'u',L'P',L'h',L'k',L'o',L'l',L'y',L'n',L'j',L'p',L'l',L'b',L'm',L'l',L'l' };
	//종성
	TArray<TCHAR>KrLeg = { '\x00', L'ㄱ', L'ㄲ', L'ㄳ', L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ', L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ', L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ', L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };
	TArray<TCHAR>ENLeg1 = { '\x00', L'r',L'R', L'r', L's', L's', L's', L'e', L'f',L'f',L'f',L'f',L'f',L'f',L'f',L'f',L'a',L'q',L'q',L't',L'T',L'd',L'w',L'c',L'z',L'x',L'v',L'g' };
	TArray<TCHAR>ENLeg2 = { '\x00', L'r',L'R', L't', L's', L'w', L'g', L'e', L'f',L'r',L'a',L'q',L't',L'x',L'v',L'g',L'a',L'q',L't',L't',L'T',L'd',L'w',L'c',L'z',L'x',L'v',L'g' };


	//문자열 분리
	UINT pos = 0;
	UINT posEN = 0;

	while (*str != '\0')
	{
		//영어		
		if (*str < 256)
		{
			//버퍼사이즈보다 크다면 break;
			if (pos + 2 >= nSize - 1) break;

			//버퍼에 영어 입력
			buffer[posEN] = *str;

			//포지션 이동
			++pos;
			++posEN;
		}
		//한글
		else
		{
			//버퍼사이즈보다 크다면 break
			if (pos + 4 >= nSize - 1) break;
			int AddedPos = 0;


			//초성다음이 초성인 경우
			TCHAR* tempStr = str + 1;
			if (KrHead.Contains(*tempStr) || (KrHead.Contains(*str) && *tempStr == '\0'))
			{
				//초성
				buffer[posEN] = EnHead[KrHead.IndexOfByKey(*str)];
				//포지션 역이동
				AddedPos -= 2;
			}


			else if (KrBody.Contains(*tempStr) || (KrBody.Contains(*str) && *tempStr == '\0'))
			{
				//중성: 조합된 한글
				if (KrBody[KrBody.IndexOfByKey(*str)] != KrBody[KrBody.IndexOfByKey(*str)])
				{
					buffer[posEN] = KrBody[KrBody.IndexOfByKey(*str)];
					buffer[posEN + 1] = KrBody[KrBody.IndexOfByKey(*str)];

					//포지션 역이동
					--AddedPos;
				}
				//중성: 일반한글
				else
				{
					buffer[posEN] = EnBody1[KrBody.IndexOfByKey(*str)];

					//포지션 역이동
					AddedPos -= 2;
				}
			}

			else
			{
				//초성
				buffer[posEN] = EnHead[(*str - 0xAC00) / (21 * 28)];

				//중성:조합된 한글
				if (EnBody1[(*str - 0xAC00) % (21 * 28) / 28] != EnBody2[(*str - 0xAC00) % (21 * 28) / 28])
				{
					buffer[posEN + 1] = EnBody1[(*str - 0xAC00) % (21 * 28) / 28];
					buffer[posEN + 2] = EnBody2[(*str - 0xAC00) % (21 * 28) / 28];

					++AddedPos;
				}
				//중성: 일반한글
				else 
				{
					buffer[posEN + 1] = EnBody1[(*str - 0xAC00) % (21 * 28) / 28];
				}

				//종성:조합된 한글
				if (ENLeg1[(*str - 0xAC00) % 28] != ENLeg2[(*str - 0xAC00) % 28])
				{
					buffer[posEN + 2 + AddedPos] = ENLeg1[(*str - 0xAC00) % 28];
					buffer[posEN + 3 + AddedPos] = ENLeg2[(*str - 0xAC00) % 28];

					++AddedPos;
				}
				//종성:일반 한글
				else
				{
					//종성이 없는경우
					if (ENLeg1[(*str - 0xAC00) % 28 == '\x00'])
					{
						--AddedPos;
					}
					//종성이 있는경우
					else
					{
						buffer[posEN + 2 + AddedPos] = ENLeg1[(*str - 0xAC00) % 28];
					}
				}
			}

			pos += 3;
			posEN += (3 + AddedPos);
		}
		++str;
	}
	buffer[posEN] = '\0';

	return pos;
}

void AVKActor::BackSpaceFunc()
{
	keybd_event(VK_BACK, 0, 0, 0);
}

