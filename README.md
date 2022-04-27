
# 사용방법
js파일을 test.cpp에 복사한 뒤 

**포맷팅 후 사용 (https://studio-g.atlassian.net/wiki/spaces/slotduc/pages/424509629/VSCode)
(command + K + F)**

path 변수에 test.cpp 절대경로 입력 후 사용

### mode

1 = Js -> cpp 변환

2 = 함수뽑기 (변환 후 사용)

# 주의사항
잘안되는부분은 주석처리

//개인별 수정사항
해당 주석 밑에부분은 개인적으로 수정 할 것

# 실행 전 따로 맞춰야하는 포맷
\* -> 모든문자
co
정렬 후 다음과 같은 형태일 때 실행
```
co 정렬 방법 (안되면 해당 내용 주석)
(
    바인드 옵션 삭제됨
    co 맨위 yield wait(\*); 를 제외하곤 모두 coFunctionWait(\*, [this] {});//wait 형태로 변환
)
```

```
*.addAnimationComplete(
    *, (*) = > {
        co(function \* () {
            try {
                yield wait(500);
                *
                *
                yield wait(\*);
                * 여러줄
                *
            }
            catch (error) {
                * 1줄일경우만 실행
            }
        }.bind(this));
    });

=>

*->addAnimationComplete(\*,[this](string entry) {
    coFunctionWait(500, [this] {
            *
            *
            coFunctionWait(\*, [this] {});//wait
            *
            *
        });
});
```
