
# 사용방법
js파일을 위아래 지우고 cpp로 바꾼뒤 포맷팅 후 사용

# 실행 전 따로 맞춰야하는 포맷
a.x -= 1 ,a.y += 1 형태 
--> 
= a.x - 1, = a.y + 1 형태로 바꾸고 실행

\* -> 모든문자
co 정렬 방법 
(
    바인드 옵션 삭제됨
    co 맨위 yield wait(\*); 를 제외하곤 모두 coFunctionWait(\*, [this] {});//wait 형태로 변환
)

co
정렬 후 다음과 같은 형태일 때 실행
(잘안될수도있음)
```
\*.addAnimationComplete(
    \*, (\*) = > {
        co(function \* () {
            try {
                yield wait(500);
                \*
                \*
                yield wait(\*);
                \* 여러줄
                \*
            }
            catch (error) {
                \* 1줄일경우만 실행
            }
        }.bind(this));
    });

=>

\*->addAnimationComplete(\*,[this](string entry) {
    coFunctionWait(500, [this] {
            \*
            \*
            coFunctionWait(\*, [this] {});//wait
            \*
            \*
        });
});
```