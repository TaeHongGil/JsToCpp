//
//  CSlot346PiggyAdventure.cpp
//  DoubleUCasino
//
//  Created by taehong on 2022/01/07.
//

#include "CSlot346PiggyAdventure.h"

#include "ArrayUtils::h"
#include "CDefineV2Slot.h"
#include "CIconContainer.h"
#include "CShake.h"
#include "CSimplePayTable.h"
#include "CSlot346PiggyAdventureReelWrapper.h"
#include "CSlotCommonUtils.hpp"
#include "CSlotIconHelper.hpp"
#include "CSlotStatus.h"
#include "CSpineContainer.h"

// piggyadventure -> 리소스이름
// slotName -> 슬롯이름(풀네임)
// CSlot346PiggyAdventure -> 해당 파일이름으로 변경
class CIconContainer;

using namespace cocos2d;

cocos2d::CCScene* CSlot346PiggyAdventure::scene(int objectidx, long long unitbet, long long jackpotmoney, int bExp, int jackpotPartyStatus, long long storagekey, int usercount, long long purchasecredit, long long recentjackpot, int ratio1, int ratio2, int jackpottrycount, int jackpotpartytrycount, long long boostAmount)
{
    CUserInfo::sharedInstance()->UnitBet = unitbet;
    CCScene* pScene = CCScene::create();
    CSlot346PiggyAdventure* pLayer = CSlot346PiggyAdventure::create();
    pScene->addChild(pLayer);
    pLayer->SetLoginData(objectidx, unitbet, jackpotmoney, bExp, jackpotPartyStatus, storagekey, usercount, purchasecredit, recentjackpot, ratio1, ratio2, jackpottrycount, jackpotpartytrycount, boostAmount);

    return pScene;
}

DownloadInfo* CSlot346PiggyAdventure::getDownloadInfo(const char* baseUrl)
{
    auto resourceInfo = new DownloadInfo();
    //타입 설정
    resourceInfo->slotType = MACHINE_HOO_HOO_HOO_MONKEY;
    resourceInfo->name = "piggyadventure";
    resourceInfo->url = StringUtils::format("%s/%s/%s", baseUrl, "v_6", "piggyadventure.zip");
    resourceInfo->zipSize = 18599242;
    resourceInfo->unZipSize = 23822907;
    return resourceInfo;
}

#pragma mark - 슬롯 초기화.
CSlot346PiggyAdventure::CSlot346PiggyAdventure()
{
}

CSlot346PiggyAdventure::~CSlot346PiggyAdventure()
{
}

bool CSlot346PiggyAdventure::init()
{
    // slot 구별 변수 - (필수 선언 요소)
    m_SlotName = "piggyadventure";
    m_GroupName = RM_ARP("piggyadventure");

    if (!CAnimateSlotBase::init()) {
        return false;
    }

    return true;
}

void CSlot346PiggyAdventure::InitVal()
{
    DLOG("[CSlot346PiggyAdventure::InitVal]");

    // 슬롯 특성 - 릴간격 (InitVal 호출전 세팅해야함)
    m_IconHGap = 4;
    m_IconVGap = 0;

    _isPlayAnimationTimeUpdateWinCoin = true;

    // super call
    CAnimateSlotBase::InitVal();

    // 페이라인 표시용 초기값
    int paylinePos[25][5] =
        {
            // 1
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 0},
            {2, 2, 2, 2, 2},
            {0, 1, 2, 1, 0},
            {2, 1, 0, 1, 2},
            // 6
            {1, 0, 0, 0, 1},
            {1, 2, 2, 2, 1},
            {0, 0, 1, 2, 2},
            {2, 2, 1, 0, 0},
            {1, 0, 1, 2, 1},
            // 11
            {1, 2, 1, 0, 1},
            {0, 1, 1, 1, 0},
            {2, 1, 1, 1, 2},
            {0, 1, 0, 1, 0},
            {2, 1, 2, 1, 2},
            // 16
            {1, 1, 0, 1, 1},
            {1, 1, 2, 1, 1},
            {0, 0, 2, 0, 0},
            {2, 2, 0, 2, 2},
            {0, 2, 2, 2, 0},
            // 21
            {2, 0, 0, 0, 2},
            {0, 0, 1, 0, 0},
            {2, 2, 1, 2, 2},
            {1, 0, 1, 0, 1},
            {1, 2, 1, 2, 1}};

    memcpy(m_paylinePos, paylinePos, sizeof(m_paylinePos));

    long long _multiPotFixMultiTemp[5] = {10, 20, 30, 50, 80};
    memcpy(_multiPotFixMulti, _multiPotFixMultiTemp, sizeof(_multiPotFixMulti));

    m_MaxUnitBetOrg = CMiscFunc::MakeSafeLongLong("ENC_3000000_ENC");

    m_bigWinMultiBase = 10;
}

void CSlot346PiggyAdventure::constructor(ResponseResult* slotInfo, Document* slotConfig)
{
    CAnimateSlotBase::constructor(slotInfo, slotConfig);
}

void CSlot346PiggyAdventure::_onInitializedInternal()
{
    DLOG("CSlot346PiggyAdventure::_onInitializedInternal");

    // super callㅇ
    CAnimateSlotBase::_onInitializedInternal();

    if (mLayer->mAnimationManager) {
        mLayer->mAnimationManager->runAnimationsForSequenceNamed(m_padResolution == 1 ? "pad" : "phone");
    }
    eff_container = mLayer->getVariable<CCNode*>("eff_container");
    popup_container = mLayer->getVariable<CCNode*>("popup_container");
    layer_reel_frame = mLayer->getVariable<CCNode*>("layer_reel_frame");

    initReels();
    makeUiScroll();
    _hitboxForNormal = create2DArray(_reelWidth, _reelWindowHeight, null);
    _fixedSymbolForBonus = create1DArray(_bonusReelWidth, null);
    _fixedSymbolForPyramid = create2DArray(_reelWidth, _reelWindowHeight, null);
    _fixedSymbolForOriental = create2DArray(_reelWidth, _reelWindowHeight, null);
    _fixedSymbolForAtlantis = create2DArray(atlantisReelWidth, 1, null);
    _fixedSymbolForAtlantis[0] = create1DArray(9, null);
    for (auto index = 1; index < atlantisReelWidth; index++) {
        _fixedSymbolForAtlantis[index][0] = nullptrptrptrptr;
    }
    _ui_multipot_text[0] = reel_bg.num_mini._text;
    _ui_multipot_text[1] = reel_bg.num_minor._text;
    _ui_multipot_text[2] = reel_bg.num_major._text;
    _ui_multipot_text[3] = reel_bg.num_ultra._text;
    _ui_multipot_text[4] = reel_bg.num_grand._text;

    // test
    //  _slotLoginInfo->slotInfo->slotSaveTB = "464000,464000,464000/29,29,29/30,24,30/11A5S8.0K,22A10S16K,2A5S8.0K,15A10S16K,5A50S80K,6A5S8K,"
    if (_slotLoginInfo->slotInfo->slotSaveTB && _slotLoginInfo->slotInfo->slotSaveTB != "no slotsave data") {
        initSlotSaveData(_slotLoginInfo->slotInfo->slotSaveTB);
    }
    reelBGChange("on_normal");
    _playScatterHitAnimation("stay", true);
}
});

coFunctionWait(2000, [this] {
    if (!isRecovery) {
        _playScatterHitAnimationForNormal("stay", true);
        getFeaturedInfo();
    }
});

//
#ifdef DEBUG_UI
InitDebugUI();
#endif

#ifdef DEBUG_QA_TOOL
m_qaTool = CDebugServerBaseQATool::create(false, 5, 3);
if (m_qaTool != nullptrptrptrptr) {
    m_qaTool->setPosition(ccp(200, 350));
    addChild(m_qaTool, 1000);
}
#endif
}
/**
 * 생성자
 * @constructor
 */
void CSlot346PiggyAdventure::constructor(slotInfo, slotConfig)
{
    super(slotInfo, slotConfig);

    _IdleProcessQueue = new FunctionQueue();
    _hidePaylineProcessQueue = new FunctionQueue();
    multipots = [ "", "mini", "minor", "major", "ultra", "grand" ];
    multipot = [ 0, 50, 100, 150, 300, 500 ];

    pickInfo = {};
    //싱글
    slotReelsForBonus = [];
    //일반
    slotReelsForPyramid = [];
    //싱글
    slotReelsForAtlantis = [];
    //일반
    slotReelsForOriental = [];
    allReelFrame = [ layer_reel_frame, layer_reel_frame_bonus, layer_reel_frame_pyramid, layer_reel_frame_atlantis, layer_reel_frame_oriental ];
    allReels = [ slotReels, slotReelsForBonus, slotReelsForPyramid, slotReelsForAtlantis, slotReelsForOriental ];

    _fixedSymbolForBonus = [];
    _fixedSymbolForPyramid = [];
    _fixedSymbolForOriental = [];
    _fixedSymbolForAtlantis = [];
    _openBox = [];
    _openBoxFrame = [];
    _ui_multipot_text = [];
    _tresureBox = [];
    _paytalbeInfo = [ null, null, null ];
    _payBoxTextForAtlantis = [];
    _payBoxTextForAtlantisBig = [];
    _payHitAniForAtlantis = [];
    _hitboxForNormal = [];
    remainPickInfo = [];

    _payBoxNumForAtlantis = nullptr;
    _payBoxNumForAtlantisBig = nullptr;
    _uiScroll = nullptr;
    _uiChoiceMap = nullptr;
    _exitButton = nullptr;
    _multipotPopup = nullptr;
    _uiKey = nullptr;
    _uiKeyText = nullptr;
    _countDown = nullptr;
    _infoPig = nullptr;
    _endPopup = nullptr;
    _openPopup = nullptr;
    _staySpine = nullptr;
    _uiTresureMap = nullptr;
    _payoutSpine = nullptr;
    _payoutTween = nullptr;

    countDownTimeout = nullptr;
    countDownTimeoutSound = nullptr;

    pickResultInfo = [ {}, {}, {}, {} ];
    linePayForAtlantis = [];
    paylineInfoForAtlantis = [];
    openState = [];
    boxResultForFreespin = [];
    fixedPayoutForBonus = [];
    fixedPayoutForSuper = [];
    fixedPlaylineForBonus = [];
    fixedPlaylineForSuper = [];
    hitReelForOriental = [];
    respinAwardForAtlantis = [];
    fixedPayForOriental = [];
    digHeightInfo = [ 0, 0, 0, 0, 0 ];

    spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
    _bonusReelWidth = 0;
    freespinRemainCount = 0;
    userPick = -1;
    pickCount = 0;
    pickRemainCount = 0;
    superFreespinRemainCount = 0;
    superFreespinTotalCount = 0;
    pickType = 0;
    symbolPayoutForSuper = 0;
    orientalCoinCount = 0;
    orientalMultiCount = 0;
    orientalTotalAward = 0;
    orientalTotalAwardPre = 0;
    spinStatusForAtlantis = 0;
    superTotalAward = 0;
    spinNum = 0;
    freespinTotalCount = 0;
    _pickScatterAppearCount = 0;
    _bonusScatterAppearCount = 0;
    totalPayTemp = 0;
    _avgTotalBet = 0;

    payoutType = "";

    isSuperFreespin = false;
    superFreespinStart = false;
    superFreespinEnd = false;
    isPick = false;
    pickEnd = false;
    pickStart = false;
    isFreespin = false;
    openMap = false;
    firstSpinForSuper = false;
    pickRetrigger = false;
    removeHitBoxFlag = false;
    isRecovery = false;
    // 페이아웃 발생시 페이라인을 한 라인씩 그려주는 슬롯은 주석을 풀어줌
    // _bShowSingleHitLine = true;

    paylinesForAtlantis = [
        [ 0, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 1 ],
        [ 2, 2, 2, 2, 2 ],
        [ 3, 3, 3, 3, 3 ],
        [ 4, 4, 4, 4, 4 ],
        [ 5, 5, 5, 5, 5 ],
        [ 6, 6, 6, 6, 6 ],
        [ 7, 7, 7, 7, 7 ],
        [ 8, 8, 8, 8, 8 ],
        // 9

        [ 2, 1, 0, 1, 2 ],
        [ 3, 2, 1, 2, 3 ],
        [ 4, 3, 2, 3, 4 ],
        [ 5, 4, 3, 4, 5 ],
        [ 6, 5, 4, 5, 6 ],
        [ 7, 6, 5, 6, 7 ],
        [ 8, 7, 6, 7, 8 ],
        [ 0, 1, 2, 1, 0 ],
        [ 1, 2, 3, 2, 1 ],

        [ 2, 3, 4, 3, 2 ],
        [ 3, 4, 5, 4, 3 ],
        [ 4, 5, 6, 5, 4 ],
        [ 5, 6, 7, 6, 5 ],
        [ 6, 7, 8, 7, 6 ],
        [ 0, 1, 1, 1, 0 ],
        [ 1, 2, 2, 2, 1 ],
        [ 7, 6, 6, 6, 7 ],
        [ 8, 7, 7, 7, 8 ]
    ]
}

/**
 * destroy
 */
void CSlot346PiggyAdventure::destroy(options)
{
    // 추가적으로 생성한 리소스 destroy
    super::destroy(options);
    _IdleProcessQueue.killAndDone();
    _hidePaylineProcessQueue.killAndDone();
    clearTimeout(countDownTimeout);
    clearTimeout(countDownTimeoutSound);
}

/** **************************************************************************************************
 *  [START]OVERRIDE METHODS
 *  **************************************************************************************************/

/**
 * _onInitializedInternal
 */
void CSlot346PiggyAdventure::_onInitializedInternal()
{
    findIndexFunc = function(reel)
    {
        auto reelLength = _reelArray.size();
        return CMiscUtils::getRandomIntInclusive(0, reelLength - 1);
    }
    //세로모드
    if (!DUG.common.settings.SLOT_HAS_TOPBAR) {
        DUG.common.settings.RENDERER_HEIGHT = 1014;
        DUG.DoubleUCasino.pixi_app.renderer.resize(DUG.common.settings.RENDERER_WIDTH, DUG.common.settings.RENDERER_HEIGHT);
    }
    super::_onInitializedInternal();
    //스캐터 등장한 릴을 위로 올리기 위함
    for (auto index = 0,size = layer_reel_frame.children.size(); index < size; index++) {
        layer_reel_frame.children.at(index).z = index * 1000;
    }
    initReels();
    makeUiScroll();
    _hitboxForNormal = create2DArray(_reelWidth, _reelWindowHeight, null);
    _fixedSymbolForBonus = create1DArray(_bonusReelWidth, null);
    _fixedSymbolForPyramid = create2DArray(_reelWidth, _reelWindowHeight, null);
    _fixedSymbolForOriental = create2DArray(_reelWidth, _reelWindowHeight, null);
    _fixedSymbolForAtlantis = create2DArray(atlantisReelWidth, 1, null);
    _fixedSymbolForAtlantis[0] = create1DArray(9, null);
    for (auto index = 1; index < atlantisReelWidth; index++) {
        _fixedSymbolForAtlantis[index][0] = nullptr;
    }
    _ui_multipot_text[0] = reel_bg.num_mini._text;
    _ui_multipot_text[1] = reel_bg.num_minor._text;
    _ui_multipot_text[2] = reel_bg.num_major._text;
    _ui_multipot_text[3] = reel_bg.num_ultra._text;
    _ui_multipot_text[4] = reel_bg.num_grand._text;

    // test
    //  _slotLoginInfo->slotInfo->slotSaveTB = "464000,464000,464000/29,29,29/30,24,30/11A5S8.0K,22A10S16K,2A5S8.0K,15A10S16K,5A50S80K,6A5S8K,"
    if (_slotLoginInfo->slotInfo->slotSaveTB && _slotLoginInfo->slotInfo->slotSaveTB != "no slotsave data") {
        initSlotSaveData(_slotLoginInfo->slotInfo->slotSaveTB);
    }
    coFunctionWait(2000, [this] {
            if (!isRecovery) {
                _playScatterHitAnimationForNormal("stay", true);
                getFeaturedInfo();
            }
        });
}

/**
 * @override
 * 슬롯 입장, 보너스 진입시에 필요한 가상 릴셋을 제공한다.
 */
void CSlot346PiggyAdventure::makeFakeReelSet(reelIndex)
{
    auto index = CMiscUtils::getRandomIntInclusive(0, 51 - 1);
    if (_slotLoginInfo->slotState != nullptr) {
        // 슬롯 진입시 이전 스핀 결과가 있으면 가장 최근 스핀 결과를 설정한다.
        index = _slotLoginInfo->slotState.reelBeforePredictionArray[reelIndex];
    }
    auto predicArray = [];
    auto spinType = (hasSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN)) ? 1 : 0;
    auto reelArray = _getReelArray(spinType);
    auto starIndex = reelIndex * 51;  // 릴셋 길이로 변경 필요
    auto limitIndex = starIndex + 51;
    for (auto i = 0; i < _reelWindowHeight; i++) {
        auto symbolIndex = (starIndex + index + i);
        if (symbolIndex >= limitIndex) {
            symbolIndex = symbolIndex - 51;
        }
        predicArray.push_back(reelArray[symbolIndex]);
    }
    return [predicArray];
}

void CSlot346PiggyAdventure::makeFakeReelSetBonus()
{
    auto predicArray = [];
    for (auto i = 0; i < _reelWindowHeight; i++) {
        predicArray.push_back(CMiscUtils::getRandomIntInclusive(10004, 10011));
    }
    return [predicArray];
}

/**
 * onSpin
 */
void CSlot346PiggyAdventure::onSpin()
{
    _skip = false;
    _bonusScatterAppearCount = 0;
    _pickScatterAppearCount = 0;
    scatterAppearForAtlantis = false;
    _IdleProcessQueue.killAndDone();
    _hidePaylineProcessQueue.killAndDone();
    clearTimeout(countDownTimeout);
    clearTimeout(countDownTimeoutSound);

    // 모든 아이콘 알파값 복원
    _setResultIconsAlpha(1);
    if (removeHitBoxFlag) {
        removeHitBoxForNormal();
        removeHitBoxFlag = false;
    }
    // 응답 지연 처리 이벤트 등록
    eventListener.on(this, "responseDelayStart", _onResponseDelayStartedBindMe ? _onResponseDelayStartedBindMe : _onResponseDelayStartedBindMe = _onResponseDelayStarted.bind(this));

    if (isPick && !isSuperFreespin) {
        removeBottomUIEffect();
        pickCount--;
        _uiKeyText->setString(StringUtils::toString(pickCount))
        _uiKeyText->setAnchorPoint(ccp(0.5, 0.5));
        if (pickCount == 0) {
            pickEnd = true;
        }
        onFakeSpin();
    }
    else {
        if (isSuperFreespin) {
            setAvgTotalBet();
            superFreespinRemainCount--;
            setFreespinLeftcount(superFreespinRemainCount, superFreespinTotalCount);
            if (superFreespinRemainCount == 0) {
                superFreespinEnd = true;
                if (pickCount > 0) {
                    pickStart = true;
                    pickRetrigger = true;
                }
            }
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            freespinRemainCount--;
            setFreespinLeftcount(freespinRemainCount, freespinTotalCount);
        }
        super::onSpin();
    }
}

void CSlot346PiggyAdventure::onSpinStart()
{
    _currentSpinIntervalTime = (_bSpinFast == true) ? _spinFastIntervalTime : _spinIntervalTime;
    _currentSpinBaseTime = (_bSpinFast == true) ? _spinFastBaseTime : _spinBaseTime;
    auto reelStopSpeed = (_bSpinFast == true) ? CSlotReelWrapper.STOP_SPEED.FAST : CSlotReelWrapper.STOP_SPEED.NORMAL;
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        super::onSpinStart();
    }
    else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
            if (_fixedSymbolForBonus[reelIndex]) {
                continue;
            }
            auto _reel = _getTypeReel(reelIndex, spinType);
            _reel->_slotReelWrapper->spin();
            auto _duration_base = _currentSpinBaseTime;
            _reel->_duration_gap = 0.05 * reelIndex;
            _reel->_slotReelWrapper->_timeline.to(_reel->_slotReelWrapper, _duration_base + _reel->_duration_gap, {
                onComplete : onCompleteSpinPhaseBindMe ? onCompleteSpinPhaseBindMe : (onCompleteSpinPhaseBindMe = onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ _reel, reelStopSpeed ]
            });
            _reel->_spin_phase = "SPIN";
            _spinActiveReel.push_back(_reel);
        }
    }
    else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
            auto _reel = _getTypeReel(reelIndex, spinType);
            auto count = 0;
            for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                    count++;
                }
            }
            if (count == _reelWindowHeight) {
                continue;
            }
            _reel->_slotReelWrapper->spin();
            auto _duration_base = _currentSpinBaseTime;
            _reel->_duration_gap = _currentSpinIntervalTime * reelIndex;
            _reel->_slotReelWrapper->_timeline.to(_reel->_slotReelWrapper, _duration_base + _reel->_duration_gap, {
                onComplete : onCompleteSpinPhaseBindMe ? onCompleteSpinPhaseBindMe : (onCompleteSpinPhaseBindMe = onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ _reel, reelStopSpeed ]
            });
            _reel->_spin_phase = "SPIN";
            _spinActiveReel.push_back(_reel);
        }
    }
    else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        for (auto reelIndex = 0; reelIndex < atlantisReelWidth; reelIndex++) {
            if (reelIndex == 0) {
                continue;
            }
            if (_fixedSymbolForAtlantis[reelIndex][0]) {
                continue;
            }
            auto _reel = _getTypeReel(reelIndex, spinType);

            _reel->_slotReelWrapper->spin();

            auto _duration_base = _currentSpinBaseTime;
            _reel->_duration_gap = 0.05 * reelIndex;
            _reel->_slotReelWrapper->_timeline.to(_reel->_slotReelWrapper, _duration_base + _reel->_duration_gap, {
                onComplete : onCompleteSpinPhaseBindMe ? onCompleteSpinPhaseBindMe : (onCompleteSpinPhaseBindMe = onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ _reel, reelStopSpeed ]
            });
            _reel->_spin_phase = "SPIN";
            _spinActiveReel.push_back(_reel);
        }
    }
    else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
            auto _reel = _getTypeReel(reelIndex, spinType);

            _reel->_slotReelWrapper->spin();

            auto _duration_base = _currentSpinBaseTime;
            _reel->_duration_gap = _currentSpinIntervalTime * reelIndex;
            _reel->_slotReelWrapper->_timeline.to(_reel->_slotReelWrapper, _duration_base + _reel->_duration_gap, {
                onComplete : onCompleteSpinPhaseBindMe ? onCompleteSpinPhaseBindMe : (onCompleteSpinPhaseBindMe = onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ _reel, reelStopSpeed ]
            });
            _reel->_spin_phase = "SPIN";
            _spinActiveReel.push_back(_reel);
        }
    }
}

void CSlot346PiggyAdventure::onSpinAfter()
{
    super::onSpinAfter();
    if (_bSpinAuto == true) {
        stopEffectSound("spin_reel");
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        if (_bSpinFast == true) {
            playEffectSound("snd_reel_fast_1", "spin_reel");
        }
        else {
            if (spinNum > 3) {
                spinNum = 1;
            }
            playEffectSound("snd_reel_spin_" + spinNum, "spin_reel");
            spinNum++;
        }
    }
}

void CSlot346PiggyAdventure::_endCurrentSpin()
{
    DUG.log("end of spin");
    // 튜토리얼 진행중인 경우 처리
    if (DUG.common.userInfo.is_tutorial_slot == true) {
        tutorialAward += _currentResult->spinResult->payout->totalAward;

        auto isTutorialEndSpin = _currentResult->spinResult->slotState.featuredMap.te || 0;
        if (isTutorialEndSpin == 1) {
            _spinProcessQueue->add(onExitToLobbyBindMe ? onExitToLobbyBindMe : (onExitToLobbyBindMe = onExitToLobby.bind(this)));
            _spinProcessQueue->next(500);
            _bSpinAuto = false;
            return;
        }
        else {
            _bSpinAuto = true;
        }
    }

    if (isPick == true) {
        DUG.log("pick");
        _processEndSpin();
    }
    else if (isNextSpinNoMoneyIn()) {
        // 프리스핀
        DUG.log("no moneyIn");
        _spinProcessQueue->add(onAutoNextSpinStartBindMe ? onAutoNextSpinStartBindMe : (onAutoNextSpinStartBindMe = onAutoNextSpinStart.bind(this)));
        _spinProcessQueue->next(500);
    }
    else if (_bSpinAuto == true) {
        DUG.log("auto spin");
        _spinProcessQueue->add(onAutoNextSpinStartBindMe ? onAutoNextSpinStartBindMe : (onAutoNextSpinStartBindMe = onAutoNextSpinStart.bind(this)));
        _spinProcessQueue->next(500);
    }
    else {
        DUG.log("init _bSpin");
        _processEndSpin();
    }
}

void CSlot346PiggyAdventure::onEndCurrentSpin()
{
    super::onEndCurrentSpin();
    DUG.common.settings.testMode = 0;
    DUG.common.settings.testMode2 = 0;
    DUG.common.settings.testMode3 = 0;
    DUG.common.settings.resetSave = 0;
    isClick = false;
    if (isRecovery) {
        isRecovery = false;
    }
    //빅윈 후 종료버튼누를때 같이 클릭되는 현상 수정
    if (isPick) {
        coFunctionWait(100, [this] {
                for (auto index = 0,size = _tresureBox.size(); index < size; index++) {
                    if (_tresureBox[index]) {
                        _tresureBox[index].setClick(true);
                    };
                }
            });
    }
}

/**
 * complete lazy load"s Event Callback
 * @override
 * 리소스 로드가 끝나지 않은채 _onCheckBonusModeSevralProc() 호출될 경우 BGM 이 정상적으로 재생되지 않아 리소스 로드 완료시 보너스 모드인 경우 바꿔 재생되도록 오버라이드
 */
void CSlot346PiggyAdventure::onCompleteLazyLoadingResources(resources)
{
    super::onCompleteLazyLoadingResources(resources);
    coFunctionWait(100, [this] {
            auto delay = 100;
            if (hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN) == false &&
                hasNextSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN) == false) {
                playEffectSound("snd_welcome");
                delay = 3000;
            }
            coFunctionWait(delay, [this] {});//wait
            if (hasNextSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN) == true) {
                playBGMSound("bgm_bonus");
            }
            else if (hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN) == true) {
                if (isPick) {
                    playBGMSound("bgm_pick_" + String.fromCharCode(pickType + 96));
                }
                if (isSuperFreespin) {
                    playBGMSound("bgm_super_" + String.fromCharCode(pickType + 96));
                }
            }
            else {
                playBGMSound("bgm_normal_reel");
            }
        });
}

/**
 * spin 동작의 End 체크 함수.
 * @param {Object} _reel - reel instance
 * @param {number} stopSpeed - speed
 * @protected
 */
void CSlot346PiggyAdventure::onCompleteSpinPhase(_reel, stopSpeed = CSlotReelWrapper.STOP_SPEED.NORMAL)
{
    if (_bSpinResponse == false) {
        if (_spinResponseLazyCount == 0) {
            emit("responseDelayStart", this);
        }
        emit("responseDelaying", this, _spinResponseLazyCount);
        // 응답을 받지 못한 경우 lazycount 증가
        _spinResponseLazyCount++;
    }
    else {
        // 정상적인 상황
        auto _reelIndex = _reel->_slotReelWrapper->reelIndex;
        auto _changeReelIndex = _reelIndex;
        auto _playLines = [];
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            _playLines = _currentResult->spinResult->reels[_changeReelIndex].playLines;
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            _changeReelIndex = _reelIndex / _reelWindowHeight;
            auto iconId = _currentResult->spinResult->reels[_changeReelIndex].playLines[_reelIndex % 9];
            if (iconId == 0) {
                iconId = CMiscUtils::getRandomIntInclusive(10004, 10011);
            }
            _playLines.push_back(iconId);
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            _playLines = _currentResult->spinResult->reels[_changeReelIndex].playLines;
            for (auto index = 0,size = _playLines.size(); index < size; index++) {
                if (_playLines[index] < 200) {
                    _playLines[index] = CMiscUtils::getRandomIntInclusive(10004, 10011);
                }
            }
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            if (_reelIndex == 0) {
                _playLines = _currentResult->spinResult->reels[_changeReelIndex].playLines;
            }
            else {
                _changeReelIndex = (_reelIndex - 1) / _reelWindowHeight + 1;
                auto iconId = _currentResult->spinResult->reels[_changeReelIndex].playLines[(_reelIndex - 1) % 9] if (iconId == 0)
                {
                    iconId = CMiscUtils::getRandomIntInclusive(10004, 10011);
                }
                else
                {
                    if (_fixedSymbolForAtlantis[_reelIndex][0]) {
                        iconId = CMiscUtils::getRandomIntInclusive(10004, 10011);
                    }
                    else {
                        iconId = 300;
                    }
                }
                _playLines.push_back(iconId);
            };
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            _playLines = _currentResult->spinResult->reels[_changeReelIndex].playLines;
            auto scatter = 0;
            auto coin = 0;
            for (auto index = 0,size = _playLines.size(); index < size; index++) {
                if (_playLines[index] == 420) {
                    scatter = index;
                    break;
                }
                if (_playLines[index] >= 430) {
                    coin = index;
                }
            }
            if (coin < scatter) {
                hitReelForOriental.push_back(_changeReelIndex);
            }
        }
        onAnimateSpinStop(_reel, _playLines, stopSpeed);
    }
}

/**
 * @override
 */
void CSlot346PiggyAdventure::onAnimatedSpinStop(_reel)
{
    auto _delay = 0;

    if (_currentResult && _currentResult->spinResult) {
        for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
            auto _reel = _getTypeReel(reelIndex, spinType);
            auto playlines = _currentResult->spinResult->reels[reelIndex].playLines;
            auto appearScatter = false for (auto rowIndex = 0,size = playlines.size(); rowIndex < size; rowIndex++)
            {
                if (isNormalScatter(playlines[rowIndex])) {
                    appearScatter = true;
                }
            }
            if (appearScatter) {
                _reel->z = reelIndex + 10;
            }
            else {
                _reel->z = reelIndex;
            }
        }
        layer_reel_frame.children.sort(depthCompare);
    }

    super::onAnimatedSpinStop(_reel, _delay);
    if (_spinActiveReel.size() == 0) {
        stopEffectSound("spin_reel");
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        _reel->_slotReelWrapper->hideNeedlessIcon();
    }
}

/**
 * 스핀 응답 지연 시작
 */
void CSlot346PiggyAdventure::_onResponseDelayStarted()
{
    eventListener.off(this, "responseDelayStart", _onResponseDelayStartedBindMe);

    eventListener.on(this, "responseDelayEnd", _onResponseDelayEndedBindMe ? _onResponseDelayEndedBindMe : _onResponseDelayEndedBindMe = _onResponseDelayEnded.bind(this));

    // 스핀 응답 지연에 필요한 처리 기능 구현
}

/**
 * 스핀 응답 지연 종료
 */
void CSlot346PiggyAdventure::_onResponseDelayEnded()
{
    eventListener.off(this, "responseDelayEnd", _onResponseDelayEndedBindMe);
}

/**
 * @override
 */
void CSlot346PiggyAdventure::_initPayoutComponents(options)
{
    options = options || {};
    super::_initPayoutComponents(options);
}

/**
 * @override
 * 페이아웃 전 필요한 처리
 */
void CSlot346PiggyAdventure::onRespinseSpinResultBeforePayout()
{
}

/**
 * @override
 * 다음 스핀 요청에 대한 응답 처리
 */
void CSlot346PiggyAdventure::onResponseSpinResult(response)
{
    super::onResponseSpinResult(response);

    eventListener.off(this, "responseDelayStart", _onResponseDelayStartedBindMe);

    if (_currentResult->spinResult->payout->totalAward > 0) {
        // 페이아웃 연출 종료 시점에 획득금액 업데이트 요청
        _spinProcessQueue->insertBefore(onEndCurrentSpinBindMe, onUpdateUserInfoBindMe ? onUpdateUserInfoBindMe : (onUpdateUserInfoBindMe = onUpdateUserInfo.bind(this)));
    }
}

/**
 * @protected
 * _playHitAnimation
 */
void CSlot346PiggyAdventure::_playHitAnimation()
{
    auto _hitIcons = _findAllHitIcons();

    if (_hitIcons.size() < 1) {
        return;
    }
    removeHitBoxFlag = true;
    for (auto reelIndex = 0,size = slotReels.size(); reelIndex < size; reelIndex++) {
        auto _slotReelWrapper = slotReels[reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.NORMAL);
        for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            if (ArrayUtils::contains(_hitIcons, _icon)) {
                _playIconHitAnimation(_icon);
                _hitboxForNormal[reelIndex][rowIndex] = _spineContainer->borrow("s346_ui_hit_box");
                layer_reel_frame->addChild(_hitboxForNormal[reelIndex][rowIndex]);
                _hitboxForNormal[reelIndex][rowIndex]->setAnimation("hit", true);
                _hitboxForNormal[reelIndex][rowIndex]->adjustPosition(_reel, rowIndex, 1);
            }
            else {
                _playIconNoHitAnimation(_icon);
            }
        }
    }
}

void CSlot346PiggyAdventure::removeHitBoxForNormal()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            if (_hitboxForNormal[reelIndex][rowIndex]) {
                _hitboxForNormal[reelIndex][rowIndex]->removeFromParent();
                _hitboxForNormal[reelIndex][rowIndex] = nullptr;
            }
        }
    }
}

/**
 * _playScatterHitAnimation
 */
void CSlot346PiggyAdventure::_playScatterHitAnimationForNormal(_action, _loop = false, scatter = 0)
{
    if (scatter == 1) {
        playEffectSound("snd_scatter_hit");
    }
    if (scatter == 2) {
        playEffectSound("snd_scatter_hit_2");
    }
    for (auto reelIndex = 0,size = slotReels.size(); reelIndex < size; reelIndex++) {
        auto _slotReelWrapper = slotReels[reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            auto _iconId = _icon->_dug_icon_id;
            if (_action == "hit") {
                _playIconNoHitAnimation(_icon);
            }
            if (!isNormalScatter(_iconId)) {
                continue;
            }
            if (scatter == 1 && _iconId >= 30) {
                continue;
            }
            if (scatter == 2 && _iconId < 30) {
                continue;
            }
            _playIconNoHitAnimation(_icon, 1)
                auto _aniName = "hit";
            auto target = allReelFrame[spinType].animation;
            if (rowIndex != 0 && rowIndex != 8) {
                target = layer_reel_up;
            }
            auto _aIcon = _playIconAnimation({
                icon : _icon,
                action : _action,
                loop : _loop,
                aniName : _aniName,
                animationTarget : target,
                reelIndex : reelIndex,
                rowIndex : rowIndex
            });
        }
    }
}

/**
 * 메타에서 슬롯으로 제어가 넘어올 경우 체크하는 콜백함수
 */
void CSlot346PiggyAdventure::onTransferControl()
{
    _topBarControl = false;
    if (isSpinOnTransferControl()) {
        if (!isPick) {
            onSpin();
        }
        else {
            if (DUG.common.settings.SLOT_HAS_TOPBAR) {
                _isAvailableClickTopbar = false;
                _topBar.checkSpinStatusUI({isRunSpin : true});
            }
            _spinProcessQueue->add(onWaitPick.bind(this));
            _spinProcessQueue->next();
        }
    }
    else {
        DUG.common.log.warn("[onTransferControl] It is not possible to spin.");
    }
}

void CSlot346PiggyAdventure::_playIconAnimation(options)
{
    auto _icon = options.icon,
        _animationTarget = options.animationTarget || allReelFrame[spinType].animation,
        _action = options.action,
        _animationClassType = options.animationClassType,
        _reelIndex = options.reelIndex,
        _rowIndex = options.rowIndex,
        _size = options.size || 1,
        _iconVisible = options.iconVisible == true,
        _autoRelease = options.autoRelease == true;

    options.animationTarget = _animationTarget;

    // if (!_icon) {
    // 	_icon = slotReels[_reelIndex].getResultIcon(_rowIndex);
    // 	options.icon = _icon;
    // }

    auto _reel = _getTypeReel(_icon->_reelIndex, spinType),
        _aIcon = nullptr;

    _reelIndex = _icon->_reelIndex;
    _rowIndex = _icon->_rowIndex;
    if (_animationClassType) {
        // 아이콘 컴포넌트 위치에 다른 아이콘을 넣을 경우
        // 기존 아이콘 숨기고
        _icon->setVisible(_iconVisible);
        _aIcon = _iconContainer->borrowByClassType(_animationClassType);
        _aIcon->gotoAndPlay(_action);
        _icon->_dug_animation_icon = _aIcon;

        _animateAnimationOhterIcons.push_back(_icon);
    }
    else if (_icon->timeline && _icon->timeline._labels[_action] != undefined) {
        // 히트 연출을 포함하는 심볼이면
        _icon->gotoAndPlay(_action);
        _icon->_dug_previous_parent = _icon->parent;
        _icon->_dug_previous_x = _icon->getPositionX();
        _icon->_dug_previous_y = _icon->getPositionY();
        _aIcon = _icon;

        _animateAnimationIcons.push_back(_icon);
    }
    else if (_icon->playAnimaiton) {
        _aIcon = _icon->playAnimaiton(options);
        _animationIcons.push_back(_icon);
    }

    if (_aIcon == nullptr) {
        // 히트 연출을 포함하지 않는 심볼이면 컨테이너에서 빌려옴
        // 기존 아이콘 숨기고
        _icon->setVisible(_iconVisible);
        // 히트 연출 아이콘 추가
        _aIcon = _animationIconContainer->borrow(_icon->_dug_icon_id, _action);
        _aIcon->loop = options.loop == true;
        _aIcon->gotoAndPlay(0);
        if (_autoRelease == false) {
            _icon->_dug_animation_icon = _aIcon;
        }
        _aIcon->onComplete = function()
        {
            DUG.log("onComplete() : " + _aIcon->_dug_icon_id);
            if (_autoRelease == true) {
                _aIcon->stop();
                _aIcon->parent.removeChild(_aIcon);
                _animationIconContainer.release(_aIcon);
                _icon->setVisible(true);
            }
        }
        .bind(this);
        _spritesheetAnimationIcons.push_back(_icon);
    }
    _rowIndex = checkTopBottomForLongSymbol(_icon, _rowIndex);
    if (_aIcon->adjustPosition) {
        _aIcon->adjustPosition(_reel, _rowIndex, _size);
    }
    else {
        _aIcon->setPositionX(_reel->getPositionX() + (_iconWidth - _aIcon->width) / 2);
        _aIcon->setPositionY(_reel->getPositionY() + (_iconHeight * _rowIndex) + ((_iconHeight * _size) - _aIcon->height) / 2);
    }
    _animationTarget->addChild(_aIcon);
    _aIcon->_reelIndex = _reelIndex;
    _aIcon->_rowIndex = _rowIndex;
    return _aIcon;
}

void CSlot346PiggyAdventure::checkTopBottomForLongSymbol(_icon, _rowIndex)
{
    if (spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        return _rowIndex;
    }
    auto _iconId = _icon->_dug_icon_id;
    if (_rowIndex == 0 && (_iconId == 22 || _iconId == 32)) {
        _rowIndex -= 1;
    }
    if (_rowIndex == 8 && (_iconId == 20 || _iconId == 30)) {
        _rowIndex += 1;
    }
    return _rowIndex;
}

void CSlot346PiggyAdventure::onIdleSymbol()
{
    _spinProcessQueue->next(1);

    if (isSuperFreespin || isPick || isFreespin) return;
    if (_bSpinAuto == true) return;
    if (_currentResult == undefined) return;
    if (_currentResult->spinResult->payout->totalAward == 0 && _currentResult->spinResult->payout->isJackpot == false) {
        if (_IdleProcessQueue) {
            _IdleProcessQueue.killAndDone();
            _IdleProcessQueue.add(
                onIdleSymbolIconEffectBindMe
                    ? onIdleSymbolIconEffectBindMe
                    : (onIdleSymbolIconEffectBindMe =
                           onIdleSymbolIconEffect.bind(this)));
            _IdleProcessQueue.next(700);
        }
        else {
            onIdleSymbolIconEffect();
        }
    }
}

void CSlot346PiggyAdventure::onIdleSymbolIconEffect()
{
    _IdleProcessQueue.killAndDone();
    _initAllAnimations();
    _playScatterHitAnimationForNormal("stay", true);
}

void CSlot346PiggyAdventure::isNormalScatter(_iconId)
{
    return _iconId == 20 || _iconId == 21 || _iconId == 22 || _iconId == 30 || _iconId == 31 || _iconId == 32;
}
void CSlot346PiggyAdventure::isNormalFreespinScatter(_iconId)
{
    return _iconId == 20 || _iconId == 21 || _iconId == 22;
}

void CSlot346PiggyAdventure::initReels()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
        auto _reelArray = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 203, 210 ];

        _reel->_slotReelWrapper = _createSlotReelWrapperForPyramid(_reel, {
            slotConfig : _slotConfig,
            iconContainer : _iconContainer,
            reelIndex : reelIndex,
            reelArray : _reelArray,
            reelWindowHeight : _reelWindowHeight,
            iconHeight : _iconHeight,
            startPosition : 0,
            findIndexFunc : findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : _iconLayerOrder,
            fakeReelSet : makeFakeReelSetBonus()
        });
        slotReelsForPyramid[reelIndex] = _reel->_slotReelWrapper;
        slotReelsForPyramid[reelIndex]->setVisible(false);
    }

    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);
        auto _reelArray = [ 420, 430, 430, 430, 430, 430, 430, 430, 430, 440 ];
        _reel->_slotReelWrapper = _createSlotReelWrapperForOriental(_reel, {
            slotConfig : _slotConfig,
            iconContainer : _iconContainer,
            reelIndex : reelIndex,
            reelArray : _reelArray,
            reelWindowHeight : _reelWindowHeight,
            iconHeight : _iconHeight,
            startPosition : 0,
            findIndexFunc : findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : _iconLayerOrder,
            fakeReelSet : [ 430, 430, 430, 430, 430, 430, 430, 430, 430 ]
        });
        slotReelsForOriental[reelIndex] = _reel->_slotReelWrapper;
        slotReelsForOriental[reelIndex]->setVisible(false);
    }

    _bonusReelWidth = _reelWidth * _reelWindowHeight;
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        auto _reelArray = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 50, 51, 52, 53, 54, 55, 60 ];
        auto _reelArrayTop = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 40, 41, 42, 45 ];
        _reel->_slotReelWrapper = _createSlotReelWrapperForBonus(_reel, {
            slotConfig : _slotConfig,
            iconContainer : _iconContainer,
            reelIndex : reelIndex,
            reelArray : reelIndex % 9 == 0 ? _reelArrayTop : _reelArray,
            reelWindowHeight : 1,
            iconHeight : _iconHeight,
            startPosition : 0,
            findIndexFunc : findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : _iconLayerOrder,
            fakeReelSet : [10005]
        });
        slotReelsForBonus[reelIndex] = _reel->_slotReelWrapper;
        if (reelIndex % 9 != 0) {
            _reel->_slotReelWrapper->getPositionY() += 6;
        }
        _reel->_slotReelWrapper->hideNeedlessIcon();
    }

    atlantisReelWidth = _reelWindowHeight * 3 + 1;
    for (auto reelIndex = 0; reelIndex < atlantisReelWidth; reelIndex++) {
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
        auto _reelArrayFirst = [ 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000 ];
        auto _reelArray = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 300 ];
        _reel->_slotReelWrapper = _createSlotReelWrapperForAtlantis(_reel, {
            slotConfig : _slotConfig,
            iconContainer : _iconContainer,
            reelIndex : reelIndex,
            reelArray : reelIndex == 0 ? _reelArrayFirst : _reelArray,
            reelWindowHeight : reelIndex == 0 ? _reelWindowHeight : 1,
            iconHeight : _iconHeight,
            startPosition : 0,
            findIndexFunc : findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : _iconLayerOrder,
            fakeReelSet : reelIndex == 0 ? makeFakeReelSetBonus() : [10005]
        });
        slotReelsForAtlantis[reelIndex] = _reel->_slotReelWrapper;
        slotReelsForAtlantis[reelIndex]->setVisible(false);
    }
    for (auto index = 1,size = allReelFrame.size(); index < size; index++) {
        allReelFrame[index]->setVisible(false);
    }
}

/**
 * 해당 인덱스 릴 인스턴스 반환
 * @protected
 *
 * @param {number} reelIndex - 릴 인덱스 (0 부터 시작)
 * @returns {Object} 릴 객체
 */
void CSlot346PiggyAdventure::_getTypeReel(reelIndex, type)
{
    if (type == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        return layer_reel_frame["reel_" + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        return layer_reel_frame_bonus["reel_" + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        return layer_reel_frame_pyramid["reel_" + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        return layer_reel_frame_atlantis["reel_" + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        return layer_reel_frame_oriental["reel_" + (reelIndex + 1)];
    }
}

void CSlot346PiggyAdventure::setMultiPotText()
{
    for (auto index = 0,size = _ui_multipot_text.size(); index < size; index++) {
        auto fixPay = _totalBet * multipot[index + 1] / 10;
        if (isSuperFreespin) {
            fixPay = _avgTotalBet * multipot[index + 1] / 10;
        }
        _ui_multipot_text[index]->setString(CMiscUtils::moneyFormatter(fixPay))
        _ui_multipot_text[index]->setAnchorPoint(ccp(0.5, 0.5));
    }
}

void CSlot346PiggyAdventure::setReelVisible(type)
{
    for (auto index = 0,size = allReelFrame.size(); index < size; index++) {
        if (index == type) {
            allReelFrame[index]->setVisible(true);
        }
        else {
            allReelFrame[index]->setVisible(false);
        }
    }
}

/**
 * 페이아웃 증가 Update에 대한 Callback
 *  @protected
 *  @param {number} value - update 되는 값.
 */
void CSlot346PiggyAdventure::onPayoutUpdateValue(value)
{
    // 페이아웃 스킵 확인
    if (_bUsePayoutSkip == false) {
        super::onPayoutUpdateValue(value);
    }
    else {
        if (_bPayoutSkipping == true) {
            if (_timeline_payout) {
                _timeline_payout->kill();
                _timeline_payout->clear();
            }
            onPayoutCompleteValue(value, value.options.onComplete, value.options.nextQueueDelay);
        }
        else {
            if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
                super::onPayoutUpdateValue(value);
            }
            // 보너스 팟 공통화
            if (_bUse_update_bonus_pot == true) {
                auto _totalAward = value.init + value.value * (value.result - value.init);
                _incremental_bonus_pot = _incremental_bonus_pot_end + _totalAward;
                bonuspot_number = _incremental_bonus_pot;
            }
        }
    }
}

/**
 * @protected
 * 결과 아이콘들의 알파값을 설정
 */
void CSlot346PiggyAdventure::_setResultIconsAlpha(alpha)
{
    auto width = slotReels.size();
    for (auto reelIndex = 0; reelIndex < width; reelIndex++) {
        auto _slotReelWrapper = slotReels[reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        auto _beforeIcons = _slotReelWrapper->_beforeIcons;
        for (auto rowIndex = 0, length = _resultIcons.size(); rowIndex < length; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            _icon->setVisible(true);
            _icon->setOpacity(255 * alpha);
        }
        for (auto rowIndex = 0, length = _beforeIcons.size(); rowIndex < length; rowIndex++) {
            auto _icon = _beforeIcons[rowIndex];
            _icon->setVisible(true);
            _icon->setOpacity(255 * alpha);
        }
    }
}

void CSlot346PiggyAdventure::checkReelDelay(responseResult)
{
    if (firstSpinForSuper == true) {
        getFeaturedInfo();
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            superFreespinStartAnimationForPyramid();
            for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
                auto delay = reelIndex > 0 ? 4000 : 2000;
                setReelDelayTime(reelIndex, delay);
            }
            _bSpinReelDelaying = true;
        }
        firstSpinForSuper = false;
    }
}

void CSlot346PiggyAdventure::onBigwinBefore()
{
    isClick = true;
    removeEndPopup();
    if (spinType != CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS && !isPick) {
        playEffectSpine("s346_ui_bigwin", popup_container, "hit");
        playEffectSound("snd_omen_bigwin");
    }
    else {
        pausedBGMSound(true);
    }

    coFunctionWait(0, [this]{
            _spinProcessQueue->next(1000);
        }
        catch (error) {
            errorLog(error);
            _spinProcessQueue->next();
        }
    }.bind(this));
}

void CSlot346PiggyAdventure::onBigwin(options)
{
    if (spinType != CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS && !isPick) {
        pausedBGMSound(true);
    }
    super::onBigwin(options);
}

void CSlot346PiggyAdventure::onBigwinEnd()
{
    if ((spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS || spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) && pickCount == 0) {
        setNormal();
        m_bottomPanel->setSpinAgain();
        superTotalAward = 0;
    }
    else {
        pausedBGMSound(false);
    }
    // bgm 복구
    if (hasSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN)) {
        bonuspot_number = _currentResult->spinResult->freespin->totalAward;

        // 보너스 팟 공통화 빅윈 end 시 보너스팟 갱신.
        if (_bUse_update_bonus_pot == true) {
            _incremental_bonus_pot = _incremental_bonus_pot_end = _currentResult->spinResult->freespin->totalAward;
        }
    }
    // 빅윈 엔드가 호출되면 오토스핀일때, 빅윈프로퍼티를 가져와서 딜레이 타임을 넣어준다.
    _spinProcessQueue->next(_delayAfterBigwin());
}

void CSlot346PiggyAdventure::playBlackLayer(action, parent)
{
    removeBlackLayer();
    _blackLayer = _spineContainer->borrow("s346_ui_black_layer");
    parent->addChild(_blackLayer);
    _blackLayer->setAnimation(action);
}

void CSlot346PiggyAdventure::removeBlackLayer(action = false)
{
    if (action) {
        _blackLayer->setAnimation("out");
        _blackLayer->addAnimationComplete("out",[this](string entry) {
                coFunctionWait(0, [this] {
                        removeBlackLayer(false);
                    });
            });
    }
    else {
        if (_blackLayer) {
            _blackLayer->removeFromParent();
            _blackLayer = nullptr;
        }
    }
}

void CSlot346PiggyAdventure::removeInfoMap()
{
    removeTresureBox();
    removeUiTresureMap();
    removeInfoPig(false, false);
    removeExitButton();
    removeChoiceMap();
    _uiScroll.buttonMode = true;
    _uiScroll.interactive = true;
    isClick = false;
    openMap = false;
}

/**
 * spin Click 처리
 *  @protected
 *  @returns {boolean} status
 */
void CSlot346PiggyAdventure::onClickSpin(e)
{
    if (isPick) {
        return false;
    }
    if (openMap) {
        removeInfoMap();
    }
    super::onClickSpin(e);
}

void CSlot346PiggyAdventure::onClickBetUp(e)
{
    if (isPick) {
        return false;
    }
    if (openMap) {
        removeInfoMap();
    }
    super::onClickBetUp(e);
}

void CSlot346PiggyAdventure::onClickBetDown(e)
{
    if (isPick) {
        return false;
    }
    if (openMap) {
        removeInfoMap();
    }
    super::onClickBetDown(e);
}

/**
 * maxbet 버튼 클릭 처리
 * @override
 */
void CSlot346PiggyAdventure::onClickMaxBet(e)
{
    if (isPick) {
        return false;
    }
    if (openMap) {
        removeInfoMap();
    }
    super::onClickMaxBet();
}

/**
 *  @protected
 * pay table 버튼 클릭 처리
 */
void CSlot346PiggyAdventure::onClickPaytable(e)
{
    if (isPick) {
        return false;
    }
    if (openMap) {
        removeInfoMap();
    }
    super::onClickPaytable(e);
}

/**
 *  오토스핀 클릭이 가능한 상태인지 체크
 * @override
 * @returns {boolean} status
 */
void CSlot346PiggyAdventure::_isEnableAuto()
{
    if (isPick) {
        return false;
    }
    if (openMap) {
        removeInfoMap();
    }
    return true;
}

void CSlot346PiggyAdventure::checkBetButtonStatusByBetLevel()
{
    if (isPick || isSuperFreespin) {
        m_bottomPanel->setDisableBetBtn();
    }
    else {
        super::checkBetButtonStatusByBetLevel();
    }
}
// _isEnablePayoutSkip() {
// 	if(spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL){
// 		return false;
// 	}
// 	super::_isEnablePayoutSkip();
// }
// DUC-Take5 통합 추가
void CSlot346PiggyAdventure::clickSpinSkipProgress()
{
    // 릴 스킵
    if (_isEnablePayoutSkip()) {
        if (spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            return;
        }
        onPayoutSkipProgress();
    }
    else if (_isEnableSkipSpin()) {
        onSpinSkip();
    }
}
/**
 * 터치 영역 생성
 * @protected
 */
void CSlot346PiggyAdventure::_createInteractiveRegion(x, y, w, h, color = 0x000000, alpha = 0.0)
{
    auto rv = new PIXI.Graphics();
    rv.lineStyle(1, color, 1);
    rv.beginFill(color, 1);
    rv.drawRect(-(w / 2), -(h / 2), w, h);
    rv.endFill();
    rv->setOpacity(255 * alpha);
    rv->setPositionX(x);
    rv->setPositionY(y);

    rv.hitArea = new PIXI.Rectangle(-(w / 2), -(h / 2), w, h);

    return rv;
}

void CSlot346PiggyAdventure::setResultIconVisible()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0, length = _resultIcons.size(); rowIndex < length; rowIndex++) {
            _resultIcons[rowIndex]->setVisible(true);
        }
    }
}

void CSlot346PiggyAdventure::playMultipotPopup(pay, isSuper)
{
    removeMultipotPopup();
    auto index = isSuper ? getMultipotForSuper(pay) : pay;
    _multipotPopup = _spineContainer->borrow("s346_ui_multipot_popup");
    popup_container->addChild(_multipotPopup);
    auto _text = _multipotPopup->addBitmapText("num", {size : 74, font : "s346_num_popup_multipot", align : "center", alignY : "center", alignY : "center"}, eventListener);
    _text->setString(_ui_multipot_text[index - 1].text)
    _text->setAnchorPoint(ccp(0.5, 0.5));
    coFunctionWait(500, [this] {
            playEffectSound("snd_" + multipots[index]);
        });
    _multipotPopup->setAnimation(multipots[index]);
    _multipotPopup->addAnimationComplete(multipots[index],[this](string entry) {
            coFunctionWait(0, [this] {
                    removeMultipotPopup();
                });
        });
}
////////////////////////////////////////////////////////////////////////////////////////////
//슬롯 세이브
////////////////////////////////////////////////////////////////////////////////////////////
/**
 * customFreespinEnd Callback
 * @override
 */
void CSlot346PiggyAdventure::onCustomFreespinCallback()
{
    super::onCustomFreespinCallback();
    requestGetSlotSaveString();
}

/**
 * tbl_user_slot_save 의 값 조회 리시브 성공
 * 개별 슬롯에서 오버라이드 해서 데이터를 파싱 하도록 해야함.
 */
void CSlot346PiggyAdventure::onReceiveSuccessSlotSaveString(response)
{
    initSlotSaveData(response.data);
}

void CSlot346PiggyAdventure::initSlotSaveData(slotSaveData)
{
    pickResultInfo = [ {}, {}, {}, {} ];
    auto strVal = "";
    if (slotSaveData) {
        strVal = slotSaveData.toString();
    }
    if (strVal != "" && strVal != "no slotsave data") {
        auto splitTemp = strVal.split("/");
        if (splitTemp.size() > 3) {
            auto info = splitTemp[3].split(",");
            info.forEach(string = > {
                if (string != "") {
                    if (string.indexOf("P") >= 0) {
                        auto index = string.split("P");
                        pickResultInfo["1"][index[0]] = index[1];
                    }
                    else if (string.indexOf("A") >= 0) {
                        auto index = string.split("A");
                        pickResultInfo["2"][index[0]] = index[1];
                    }
                    else if (string.indexOf("O") >= 0) {
                        auto index = string.split("O");
                        pickResultInfo["3"][index[0]] = index[1];
                    }
                }
            });
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//탐험 보너스 함수
////////////////////////////////////////////////////////////////////////////////////////////
//싱글릴
void CSlot346PiggyAdventure::_createSlotReelWrapperForBonus(instance, options)
{
    options.onBounceStopSpin = _onBounceStopSpinForBonus.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    auto randomStart = CMiscUtils::getRandomIntInclusive(0, options.reelArray.size() - 1);
    options.startPosition = randomStart;
    options.spineContainer = _spineContainer;
    options.findIndexFunc = findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureBonusReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
void CSlot346PiggyAdventure::_onBounceStopSpinForBonus(_slotReelWrapper)
{
    const reelIndex = _slotReelWrapper->reelIndex;
    // 릴 스핀 종료 재생
    auto _resultIcons = _slotReelWrapper->getResultIcons();
    auto boomAppear = false;
    auto coinAppear = false;
    auto boxAppear = false;
    auto extrAppear = false;
    for (auto rowIndex = 0; rowIndex < 1; rowIndex++) {
        auto _icon = _resultIcons[rowIndex];
        auto _iconId = _icon->_dug_icon_id;
        if (!isBonusScatter(_iconId)) {
            continue;
        }
        if (_iconId == 40 || _iconId == 41 || _iconId == 42) {
            boomAppear = true;
        }
        if (_iconId == 45) {
            extrAppear = true;
        }
        if (_iconId == 60) {
            boxAppear = true;
        }
        if (_iconId >= 50) {
            coinAppear = true;
        }
        auto _aniName = "hit";
        if (_iconId == 40 || _iconId == 41 || _iconId == 42 || _iconId == 60) {
            _aniName = "appear";
        }
        auto _aIcon = _playIconAnimation({
            icon : _icon,
            action : "appear",
            loop : false,
            aniName : _aniName,
            animationTarget : layer_reel_up,
            reelIndex : reelIndex,
            rowIndex : rowIndex
        });
        setIconTextForBonus(_aIcon, _iconId);
        if (_aIcon->_attach) {
            _aIcon->_attach->setAnimation("appear");
        }
        if ((_iconId >= 50 && _iconId <= 55) || _iconId == 60) {
            if (_openBox[reelIndex]) {
                _openBox[reelIndex]->setVisible(true);
                if (openState[reelIndex] == true) {
                    _openBox[reelIndex]->setAnimation("hold");
                    _openBox[reelIndex].open = true;
                    setTintForBonus(_icon, false);
                    setTintForBonus(_aIcon, false);
                }
                else {
                    _openBox[reelIndex]->setAnimation("off");
                    setTintForBonus(_icon, true);
                    setTintForBonus(_aIcon, true);
                }
            }
            if (_openBoxFrame[reelIndex]) {
                _openBoxFrame[reelIndex]->removeFromParent();
                _openBoxFrame[reelIndex] = nullptr;
            }
        }
    }
    playEffectSound("snd_reel_stop_2");
    if (coinAppear && !boxAppear && !boomAppear && !extrAppear) {
        playEffectSound("snd_coin_appear");
    }
    if (boxAppear && !boomAppear && !extrAppear) {
        playEffectSound("snd_box_appear");
    }
    if (boomAppear && !extrAppear) {
        playEffectSound("snd_bomb_appear");
    }
    if (extrAppear) {
        playEffectSound("snd_extra_appear");
    }
}

void CSlot346PiggyAdventure::freespinProc()
{
    _initAllAnimations();
    setOpenState();
    //고정 심볼 생성
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (!_fixedSymbolForBonus[reelIndex]) {
            auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
            auto _slotReelWrapper = slotReelsForBonus[reelIndex];
            auto _icon = _slotReelWrapper->getResultIcon(0);
            auto _iconId = _icon->_dug_icon_id;
            if ((_iconId >= 50 && _iconId <= 55) || (_iconId == 60)) {
                _icon->setVisible(false);
                auto spineName = "";
                switch (_iconId) {
                    case 50:
                        spineName = "s346_icon_coin_hit";
                        break;
                    case 60:
                        spineName = "s346_icon_mystery_box";
                        break;
                    default:
                        spineName = "s346_icon_dia_hit";
                        break;
                }
                _fixedSymbolForBonus[reelIndex] = _spineContainer->borrow(spineName);
                _fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
                _fixedSymbolForBonus[reelIndex]._rowIndex = 0;
                _fixedSymbolForBonus[reelIndex]._dug_icon_id = _iconId;
                layer_reel_up->addChild(_fixedSymbolForBonus[reelIndex]);
                _fixedSymbolForBonus[reelIndex]->adjustPosition(_reel, 0, 1);
                setIconTextForBonus(_fixedSymbolForBonus[reelIndex], _fixedSymbolForBonus[reelIndex]._dug_icon_id);
                if (_openBox[reelIndex].open == false) {
                    setTintForBonus(_fixedSymbolForBonus[reelIndex], true);
                }
                else {
                    setTintForBonus(_fixedSymbolForBonus[reelIndex], false);
                }
            }
        }
    }
    _spinProcessQueue->next(100);
}

void CSlot346PiggyAdventure::freespinProc1()
{
    // topreel stay 연출
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        auto _slotReelWrapper = slotReelsForBonus[reelIndex];
        auto _icon = _slotReelWrapper->getResultIcon(0);
        auto _iconId = _icon->_dug_icon_id;
        if (_iconId >= 40 && _iconId <= 42) {
            auto index = 0;
            for (auto i = reelIndex + 1; i < reelIndex + 9; i++) {
                if (_openBox[i].open == false) {
                    index = i;
                    break;
                };
            }
            if (index == 0) {
                continue;
            }
            auto _aIcon = _playIconAnimation({
                icon : _icon,
                action : "hit",
                loop : true,
                aniName : "stay",
                animationTarget : layer_reel_up_3,
                reelIndex : reelIndex,
            });
        }
    }
    // topreel hit연출
    coFunctionWait(0, [this]{
            for (auto reelWidth = 0; reelWidth < _reelWidth; reelWidth++) {
                auto _reelIndex = 9 * reelWidth;
                auto _reelMax = 9 * (reelWidth + 1);
                for (auto reelIndex = _reelIndex; reelIndex < _reelMax; reelIndex++) {
                    auto _slotReelWrapper = slotReelsForBonus[reelIndex];
                    auto _icon = _slotReelWrapper->getResultIcon(0);
                    auto _iconId = _icon->_dug_icon_id;
                    if (_iconId >= 40 && _iconId <= 42) {
                        auto index = 0;
                        for (auto i = _reelIndex + 1; i < _reelMax; i++) {
                            if (_openBox[i].open == false) {
                                index = i;
                                break;
                            };
                        }
                        if (index == 0) {
                            break;
                        }
                        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
                        auto spine = _spineContainer->borrow("s346_ui_focus_box");
                        layer_reel_up->addChild(spine);
                        spine->setAnimation("hit");
                        spine->adjustPosition(_reel, 0, 1);
                        spine->addAnimationComplete("hit",[this](string entry) {
                                coFunctionWait(0, [this] {
                                        spine->removeFromParent();
                                        spine = nullptr;
                                    });
                            });
                        playEffectSound("snd_bomb_hit");
                        playEffectSound("snd_bomb_hit_" + (_iconId % 40 + 1));
                        _playIconAnimation({
                            icon : _icon,
                            action : "hit",
                            animationTarget : layer_reel_up,
                            aniName : "down_" + (index % 9),
                            reelIndex : reelIndex,
                        });
                        coFunctionWait(833, [this] {});//wait
                        removeBox(reelWidth);
                        coFunctionWait(300, [this] {});//wait
                    }
                    else if (_iconId == 45) {
                        playEffectSound("snd_retrigger");
                        playEffectSpine("s346_ui_spin_add", eff_container, "hit");
                        auto _aIcon = _playIconAnimation({
                            icon : _icon,
                            action : "hit",
                            animationTarget : layer_reel_up,
                            reelIndex : reelIndex,
                        });
                        coFunctionWait(1666, [this] {});//wait
                        freespinRemainCount++;
                        freespinTotalCount++;
                        setFreespinLeftcount(freespinRemainCount, freespinTotalCount);
                        _writeSlotActionLog({
                            category_info : "",
                            action : "BONUS_RETRIGGER",
                            action_info : "Total Count: " + freespinTotalCount,
                            isplaynow : 1
                        });
                    }
                }
            }
            setOpenState();
            updateStaySymbolForBonus();
            _spinProcessQueue->next(500);
        });
}

void CSlot346PiggyAdventure::freespinPayoutProc()
{
    coFunctionWait(0, [this]{
            auto index = 0;
            for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
                if (_fixedSymbolForBonus[reelIndex]) {
                    auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
                    auto _iconId = _fixedSymbolForBonus[reelIndex]._dug_icon_id;
                    if (_iconId == 60 && _openBox[reelIndex].open == true) {
                        auto _delay = 1000;
                        if (_fixedSymbolForBonus[reelIndex]) {
                            _fixedSymbolForBonus[reelIndex]->removeFromParent();
                            _fixedSymbolForBonus[reelIndex] = nullptr;
                        }
                        //연출이 겹치는 부분이 있어 새로 생성하여 연출
                        playEffectSound("snd_box_open");
                        auto spine = _spineContainer->borrow("s346_icon_mystery_box");
                        layer_reel_up->addChild(spine);
                        spine->adjustPosition(_reel, 0, 1);
                        spine->setAnimation("hit");
                        spine->addAnimationComplete("hit",[this](string entry) {
                                coFunctionWait(0, [this] {
                                        spine->removeFromParent();
                                    });
                            });
                        coFunctionWait(1000, [this] {});//wait
                        auto changeId = boxResultForFreespin[index];
                        index++;
                        auto spineName = "s346_icon_dia_hit";
                        if (changeId == 50) {
                            spineName = "s346_icon_coin_hit";
                        }
                        if (changeId == 31 || changeId == 32) {
                            spineName = "s346_icon_key";
                        }
                        if (changeId > 50) {
                            fixedPayoutForBonus[reelIndex] = multipot[changeId % 50];
                        }
                        _fixedSymbolForBonus[reelIndex] = _spineContainer->borrow(spineName);
                        _fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
                        _fixedSymbolForBonus[reelIndex]._dug_icon_id = changeId;
                        layer_reel_up->addChild(_fixedSymbolForBonus[reelIndex]);
                        _fixedSymbolForBonus[reelIndex]->adjustPosition(_reel, 0, 1);
                        setIconTextForBonus(_fixedSymbolForBonus[reelIndex], changeId);
                        auto action = "box_open";
                        if (changeId == 31) {
                            action = "appear_2";
                        }
                        if (changeId == 32) {
                            action = "appear_3";
                        }
                        _fixedSymbolForBonus[reelIndex]->setAnimation(action);
                        if (_fixedSymbolForBonus[reelIndex]._attach) {
                            _fixedSymbolForBonus[reelIndex]._attach->setAnimation("box_open");
                        }
                        if (changeId == 31 || changeId == 32) {
                            _delay = 2000;
                        }
                        coFunctionWait(_delay, [this] {});//wait
                    }
                }
            }
            updateStaySymbolForBonus();
            _spinProcessQueue->next(500);
        });
}

void CSlot346PiggyAdventure::freespinPayoutProc1()
{
    if (m_bottomPanel) {
        removeBottomUIEffect();
    }
    // _initAllAnimations();
    setBoxDepth();
    playBlackLayer("bonus", black);
    auto slotState = getSlotState() || {};
    auto totalPayout = slotState->freespinTotalAward;
    setOnlyBonusPotPay(true);
    payoutType = "1";
    playEffectSoundWithOption("snd_payout_" + payoutType, "snd_payout_" + payoutType, true);
    changeBGMVolume(0.5);
    coFunctionWait(0, [this]{
            auto totalPay = 0;
            for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
                if (_fixedSymbolForBonus[reelIndex]) {
                    auto _icon = _fixedSymbolForBonus[reelIndex];
                    auto _iconId = _icon->_dug_icon_id;
                    if (_iconId >= 50 && _openBox[reelIndex].open == true) {
                        auto _delay = 0;
                        auto _action = "hit";
                        if (_iconId > 50) {
                            playEffectSound("snd_text_count");
                            _action = "focus";
                            _delay = 2000;
                        }
                        else {
                            playEffectSound("snd_coin_count");
                            _delay = 300;
                        }
                        auto spine = _spineContainer->borrow("s346_ui_hit_box");
                        layer_reel_up->addChild(spine);
                        spine->setAnimation("hit");
                        spine->setPositionX(_icon->getPositionX());
                        spine->setPositionY(_icon->getPositionY());
                        spine->addAnimationComplete("hit",[this](string entry) {
                                coFunctionWait(0, [this] {
                                        if (spine) {
                                            spine->removeFromParent();
                                            spine = nullptr;
                                        }
                                    });
                            });
                        _icon->setAnimation(_action);
                        if (_icon->_attach) {
                            _icon->_attach->setAnimation(_action)
                        }
                        if (_iconId > 50) {
                            auto index = _iconId % 10;
                            if (index > 0) {
                                coFunctionWait(1000, [this] {});//wait
                                playEffectSound("snd_text_popup_a");
                                playMultipotPopup(index, false);
                            }
                        }
                        totalPay += fixedPayoutForBonus[reelIndex] * _totalBet / 10;
                        bonuspot_number = totalPay;
                        if (m_bottomPanel) {
                            m_bottomPanel->playPayoutEffect();
                        }
                        coFunctionWait(_delay, [this] {});//wait
                    }
                }
            }
            stopEffectSound("snd_payout_" + payoutType);
            playEffectSound("snd_payout_" + payoutType + "_end");
            _incremental_bonus_pot_end = 0;
            _incremental_bonus_pot = totalPayout;
            bonuspot_number = totalPayout;
            changeBGMVolume(1, 1);
            _spinProcessQueue->next(2000);
        });
}

void CSlot346PiggyAdventure::freespinPayoutProc2()
{
    removeBlackLayer();
    removeMultipotPopup();
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (_fixedSymbolForBonus[reelIndex]) {
            auto _icon = _fixedSymbolForBonus[reelIndex];
            auto _iconId = _icon->_dug_icon_id;
            if (_iconId >= 50) {
                _icon->setAnimation("on");
                if (_icon->_attach) {
                    _icon->_attach->setAnimation("on")
                }
            }
        }
    }
    _spinProcessQueue->next(0);
}

void CSlot346PiggyAdventure::onStartBonusGames()
{
    freespinRemainCount = 5;
    freespinTotalCount = 5;
    _writeSlotActionLog({
        category_info : "",
        action : "BONUS_ENTER: " + freespinTotalCount,
        action_info : "",
        isplaynow : 1
    });
    _initAllAnimations();
    _changeTotalBet();
    stopBGMSound();
    setBottomUI(false);
    _playScatterHitAnimationForNormal("hit", false, 1);
    spinType = CSlot346PiggyAdventure.SPIN_TYPE.BONUS;
    _spinProcessQueue->next(2000);
}

void CSlot346PiggyAdventure::onStartBonusGames1()
{
    playBGMSound("bgm_bonus");
    playEffectSound("snd_bonus_opening_1");
    playEffectSpine("s346_ui_bonus", popup_container, "hit");
    _spinProcessQueue->next(3000);
}

void CSlot346PiggyAdventure::onStartBonusGames2()
{
    _initAllAnimations();
    playSlotStayForBonus();
    setFreespinLeftcount(freespinRemainCount, freespinTotalCount);
    setSlotMode("bonus");
    _incremental_bonus_pot = 0;
    _incremental_bonus_pot_end = 0;
    bonuspot_number = 0;
    setOnlyBonusPotPay(false);
    m_bottomPanel->setGoodLuck();
    fixBoxSymbol();
    openState = create1DArray(_bonusReelWidth, false);
    setOpenState();
    _spinProcessQueue->next(2000);
}

void CSlot346PiggyAdventure::onEndBonusGames()
{
    _spinProcessQueue->next(100);
}

void CSlot346PiggyAdventure::onEndBonusGames1()
{
    _writeSlotActionLog({
        category_info : "",
        action : "BONUS_EXIT",
        action_info : "",
        isplaynow : 1
    });

    stopBGMSound();
    playEffectSound("snd_bonus_ending_1");
    removeEndPopup();
    _endPopup = _spineContainer->borrow("s346_ui_closing");
    _endPopup->setAnimation("hit");
    popup_container->addChild(_endPopup);
    auto _text = _endPopup->addBitmapText("num", {size : 74, font : "s346_num_congratulations", align : "center", alignY : "center"}, eventListener);
    _text->setString(CMiscUtils::moneyFormatter(getSlotState()->freespinTotalAward))
    _text->setAnchorPoint(ccp(0.5, 0.5));
    _endPopup->addAnimationComplete("hit",[this](string entry) {
            coFunctionWait(0, [this] {
                    removeEndPopup();
                    _spinProcessQueue->next(0);
                });
        });
    playEffectSpine("s346_ui_payout", popup_container, "hit");
}

void CSlot346PiggyAdventure::onEndBonusGames2()
{
    if (pickType > 0 && pickCount > 0) {
        _spinProcessQueue->next(0);
    }
    else {
        setNormal();
        m_bottomPanel->setSpinAgain();
        _spinProcessQueue->next(100);
    }
}

void CSlot346PiggyAdventure::removeMultipotPopup()
{
    if (_multipotPopup) {
        _multipotPopup->removeFromParent();
        _multipotPopup = nullptr;
    }
}

void CSlot346PiggyAdventure::removeEndPopup()
{
    if (_endPopup) {
        _endPopup->removeFromParent();
        _endPopup = nullptr;
    }
}

void CSlot346PiggyAdventure::removeOpenPopup()
{
    if (_openPopup) {
        _openPopup->removeFromParent();
        _openPopup = nullptr;
    }
}

void CSlot346PiggyAdventure::removeFixedSymbolForBonus()
{
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (_fixedSymbolForBonus[reelIndex]) {
            _fixedSymbolForBonus[reelIndex]->removeFromParent();
            _fixedSymbolForBonus[reelIndex] = nullptr;
        }
    }
}

void CSlot346PiggyAdventure::removeOpenBox()
{
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (_openBox[reelIndex]) {
            _openBox[reelIndex]->removeFromParent();
            _openBox[reelIndex] = nullptr;
        }
    }
}

void CSlot346PiggyAdventure::removeOpenFrame()
{
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (_openBoxFrame[reelIndex]) {
            _openBoxFrame[reelIndex]->removeFromParent();
            _openBoxFrame[reelIndex] = nullptr;
        }
    }
}

void CSlot346PiggyAdventure::calcCoinIncreaseTimeForfreespin()
{
    auto _duration = 0;
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (_fixedSymbolForBonus[reelIndex]) {
            auto _icon = _fixedSymbolForBonus[reelIndex];
            auto _iconId = _icon->_dug_icon_id;
            if (_iconId >= 50 && _openBox[reelIndex].open == true) {
                if (_iconId > 50) {
                    _duration += 2;
                }
                else {
                    _duration += 0.3;
                }
            }
        }
    }
    return _duration + 3;
}

/*
 * 탐험 보너스 활성화칸 정보 갱신
 */
void CSlot346PiggyAdventure::setOpenState()
{
    for (auto i = 0; i < _reelWidth; i++) {
        for (auto j = 0; j < digHeightInfo[i]; j++) {
            openState[i * 9 + j + 1] = true;
        }
    }
}

void CSlot346PiggyAdventure::setBoxDepth()
{
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (_fixedSymbolForBonus[reelIndex]) {
            auto _icon = _fixedSymbolForBonus[reelIndex];
            auto _iconId = _icon->_dug_icon_id;
            if (_fixedSymbolForBonus[reelIndex]) {
                if (_iconId >= 50 && _openBox[reelIndex].open == true) {
                    layer_reel_up->addChild(_openBox[reelIndex]);
                    layer_reel_up_2->addChild(_fixedSymbolForBonus[reelIndex]);
                }
                if (_openBox[reelIndex].open == false) {
                    layer_reel_up_3->addChild(_fixedSymbolForBonus[reelIndex]);
                }
            }
        }
    }
}

void CSlot346PiggyAdventure::recoveryProcForBonus()
{
    openState = create1DArray(_bonusReelWidth, false);
    setOpenState();
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        auto _iconId = fixedPlaylineForBonus[reelIndex];
        if ((_iconId >= 50 && _iconId <= 55) || (_iconId == 60)) {
            auto spineName = "";
            switch (_iconId) {
                case 50:
                    spineName = "s346_icon_coin_hit";
                    break;
                case 60:
                    spineName = "s346_icon_mystery_box";
                    break;
                default:
                    spineName = "s346_icon_dia_hit";
                    break;
            }
            _fixedSymbolForBonus[reelIndex] = _spineContainer->borrow(spineName);
            _fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
            _fixedSymbolForBonus[reelIndex]._rowIndex = 0;
            _fixedSymbolForBonus[reelIndex]._dug_icon_id = _iconId;
        }
    }
    _openBox = create1DArray(_bonusReelWidth, null);
    _openBoxFrame = create1DArray(_bonusReelWidth, null);
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        auto _slotReelWrapper = slotReelsForBonus[reelIndex];
        _slotReelWrapper->replaceResultIconById(0, CMiscUtils::getRandomIntInclusive(10004, 10011));
        if (_fixedSymbolForBonus[reelIndex]) {
            _slotReelWrapper->getResultIcon(0)->setVisible(false);
        }
        if (reelIndex % 9 > 0) {
            _openBox[reelIndex] = _spineContainer->borrow("s346_icon_block");
            if (openState)
                _openBox[reelIndex]->setAnimation("off");
            layer_reel_down->addChild(_openBox[reelIndex]);
            _openBox[reelIndex]->adjustPosition(_reel, 0, 1);
            _openBox[reelIndex].open = false;
            if (reelIndex % 9 < 3) {
                _openBox[reelIndex]->setVisible(false);
                _openBox[reelIndex].open = true;
            }
            else {
                _openBoxFrame[reelIndex] = _spineContainer->borrow("s346_icon_block");
                _openBoxFrame[reelIndex]->setAnimation("off_frame");
                layer_reel_up_3->addChild(_openBoxFrame[reelIndex]);
                _openBoxFrame[reelIndex]->adjustPosition(_reel, 0, 1);
            }
        }
    }
    for (auto reelWidth = 0; reelWidth < _reelWidth; reelWidth++) {
        removeBox(reelWidth);
    }
    updateStaySymbolForBonus();
}
/*
 * 탐험 보너스 시작 시 히트 박스 및 고정 심볼 생성
 */
void CSlot346PiggyAdventure::fixBoxSymbol()
{
    _openBox = create1DArray(_bonusReelWidth, null);
    _openBoxFrame = create1DArray(_bonusReelWidth, null);
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        auto _slotReelWrapper = slotReelsForBonus[reelIndex];
        _slotReelWrapper->replaceResultIconById(0, CMiscUtils::getRandomIntInclusive(10004, 10011));
        if (reelIndex % 9 > 0) {
            _openBox[reelIndex] = _spineContainer->borrow("s346_icon_block");
            _openBox[reelIndex]->setAnimation("off");
            layer_reel_down->addChild(_openBox[reelIndex]);
            _openBox[reelIndex]->adjustPosition(_reel, 0, 1);
            _openBox[reelIndex].open = false;
            if (reelIndex % 9 < 3) {
                _openBox[reelIndex]->setVisible(false);
                _openBox[reelIndex].open = true;
            }
            else {
                _openBoxFrame[reelIndex] = _spineContainer->borrow("s346_icon_block");
                _openBoxFrame[reelIndex]->setAnimation("off_frame");
                layer_reel_up_3->addChild(_openBoxFrame[reelIndex]);
                _openBoxFrame[reelIndex]->adjustPosition(_reel, 0, 1);
            }
        }
        if (reelIndex % 9 <= 2) {
            _slotReelWrapper->setOpen(true);
        }
        else {
            _slotReelWrapper->setOpen(false);
        }
        if (reelIndex % 9 == 8) {
            _slotReelWrapper->replaceResultIconById(0, 60);
            _slotReelWrapper->getResultIcon(0)->setVisible(false);
            _fixedSymbolForBonus[reelIndex] = _spineContainer->borrow("s346_icon_mystery_box");
            _fixedSymbolForBonus[reelIndex]->setAnimation("in");
            _fixedSymbolForBonus[reelIndex]->addAnimationComplete("in",[this](string entry) {
                    coFunctionWait(0, [this] {
                            setTintForBonus(_fixedSymbolForBonus[reelIndex], true);
                        });
                });
            _fixedSymbolForBonus[reelIndex]->addAnimation("on");
            layer_reel_up->addChild(_fixedSymbolForBonus[reelIndex]);
            _fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
            _fixedSymbolForBonus[reelIndex]._dug_icon_id = 60;
            _fixedSymbolForBonus[reelIndex]->adjustPosition(_reel, 0, 1);
            _openBox[reelIndex]->setAnimation("off");
            if (_openBoxFrame[reelIndex]) {
                _openBoxFrame[reelIndex]->removeFromParent();
                _openBoxFrame[reelIndex] = nullptr;
            }
        }
    }
}

/*
 * 탐험보너스 stay 갱신
 */
void CSlot346PiggyAdventure::updateStaySymbolForBonus()
{
    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
        if (_fixedSymbolForBonus[reelIndex]) {
            auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
            auto _icon = _fixedSymbolForBonus[reelIndex];
            auto _iconId = _icon->_dug_icon_id;
            auto _action = "stay";
            if (_iconId == 31) {
                _action = "loop_2";
            }
            else if (_iconId == 32) {
                _action = "loop_3";
            }
            layer_reel_up->addChild(_icon);
            _icon->adjustPosition(_reel, 0, 1);
            setIconTextForBonus(_icon, _icon->_dug_icon_id);
            if (_openBox[reelIndex].open == false) {
                _action = "on";
                setTintForBonus(_icon, true);
            }
            else {
                setTintForBonus(_icon, false);
            }
            _icon->setAnimation(_action, true);
        }
    }
}

void CSlot346PiggyAdventure::setTintForBonus(_icon, tint)
{
    auto _tint = 0x8c8c8c;
    if (!tint) {
        _tint = 0xFFFFFF;
    }
    _icon->tint = _tint;
    if (_icon->_attach) {
        _icon->_attach.tint = _tint;
    }
    auto slot = _icon->getSlotContainer("num");
    if (slot && slot._text) {
        slot._text.tint = _tint;
    }
}

/*
 * 상태에 따라 탐험보너스 히트박스 지워줌
 */
void CSlot346PiggyAdventure::removeBox(reelWidth)
{
    auto _reelIndex = 9 * reelWidth + 1;
    auto _reelMax = 9 * (reelWidth + 1);
    for (auto reelIndex = _reelIndex; reelIndex < _reelMax; reelIndex++) {
        if (openState[reelIndex] == true) {
            auto _slotReelWrapper = allReels[spinType][reelIndex];
            if (_fixedSymbolForBonus[reelIndex]) {
                _openBox[reelIndex]->setVisible(true);
                _openBox[reelIndex]->setAnimation("hold");
                setTintForBonus(_fixedSymbolForBonus[reelIndex], false);
            }
            else {
                _openBox[reelIndex]->setVisible(false);
            }
            _openBox[reelIndex].open = true;
            _slotReelWrapper->setOpen(true);

            if (_openBoxFrame[reelIndex]) {
                _openBoxFrame[reelIndex]->removeFromParent();
                _openBoxFrame[reelIndex] = nullptr;
            }
        }
    }
}

void CSlot346PiggyAdventure::playSlotStayForBonus()
{
    _staySpine = _spineContainer->borrow("s346_ui_reel_change");
    eff_container->addChild(_staySpine);
    if (isRecovery) {
        _staySpine->setAnimation("loop", true);
    }
    else {
        _staySpine->setAnimation("hit");
        _staySpine->addAnimation("loop", true);
    }
}

void CSlot346PiggyAdventure::isBonusScatter(_iconId)
{
    return _iconId == 40 || _iconId == 41 || _iconId == 42 || _iconId == 45 ||
           _iconId == 50 || _iconId == 51 || _iconId == 52 || _iconId == 53 || _iconId == 54 || _iconId == 55 ||
           _iconId == 60;
}

/*
 * 탐험 보너스 아이콘 텍스트
 */
void CSlot346PiggyAdventure::setIconTextForBonus(_icon, _iconId, random = false)
{
    if (_icon == undefined || (!(_iconId >= 50 && _iconId <= 55))) {
        return;
    }
    if (_icon->_attach) {
        _icon->_attach->removeFromParent();
        _icon->_attach = nullptr;
    }
    _iconId = _iconId % 50;
    auto reelIndex = _icon->_reelIndex;
    auto fixPay = 0;
    if (random) {
        auto pay = [ 30, 25, 20, 15, 10, 5, 4, 3, 2, 1 ];
        fixPay = _totalBet * pay[CMiscUtils::getRandomIntInclusive(0, pay.size() - 1)] / 10;
    }
    else {
        fixPay = fixedPayoutForBonus[reelIndex] * _totalBet / 10;
    }
    if (_iconId == 0) {
        auto _text = _icon->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
        _text->setString(CMiscUtils::MakeUnitString(fixPay))
        _text->setAnchorPoint(ccp(0.5, 0.5));
    }
    else {
        auto spineName = "s346_icon_" + multipots[_iconId];
        auto _multipot = _spineContainer->borrow(spineName);
        _multipot->setAnimation("on");
        _icon->addChild(_multipot);
        _icon->_attach = _multipot;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 픽 보너스 함수
////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::onStartPickBonusGames()
{
    pickStart = false;
    setBottomUI(false);
    _changeTotalBet();
    if (pickRetrigger) {
        _writeSlotActionLog({
            category_info : "",
            action : "PICK RETRIGGER: " + pickCount,
            action_info : pickType,
            isplaynow : 1
        });
        _spinProcessQueue->next(0);
    }
    else {
        _writeSlotActionLog({
            category_info : "",
            action : "PICK ENTER: " + pickCount,
            action_info : pickType,
            isplaynow : 1
        });
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            makeUiKey();
        }
        _spinProcessQueue->next(500);
    }
}

void CSlot346PiggyAdventure::onStartPickBonusGames1()
{
    if (pickRetrigger) {
        _spinProcessQueue->next(0);
    }
    else {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            _initAllAnimations();
            stopBGMSound();
            removeUiScroll();
            makeUiKey();
            _playScatterHitAnimationForNormal("hit", false, 2);
            coFunctionWait(2000, [this] {
                    _uiKey->setAnimation("save");
                    _uiKeyText->setString(StringUtils::toString(pickCount))
                    _uiKeyText->setAnchorPoint(ccp(0.5, 0.5));
                });
            _spinProcessQueue->next(3333);
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            coFunctionWait(0, [this]{
                    auto count = 0;
                    for (auto reelIndex = 0; reelIndex < _bonusReelWidth; reelIndex++) {
                        if (_fixedSymbolForBonus[reelIndex]) {
                            auto _icon = _fixedSymbolForBonus[reelIndex];
                            auto iconId = _fixedSymbolForBonus[reelIndex]._dug_icon_id;
                            if (iconId == 31 || iconId == 32) {
                                auto _action = "save_2" if (iconId == 32)
                                {
                                    _action = "save_3";
                                }
                                playEffectSound("snd_key_collect");
                                layer_reel_up_2->addChild(_icon);
                                black->addChild(_uiKey);
                                _icon->setAnimation(_action);
                                _icon->addAnimationComplete(_action,[this](string entry) {
                                        coFunctionWait(0, [this] {
                                                _fixedSymbolForBonus[reelIndex]->setVisible(false);
                                            });
                                    });
                                // reelup, eff 좌표차이
                                TweenMax.to(_icon, 1.0,
                                            {
                                                ease : Power0.easeOut,
                                                x : (693 - 415) + 280.5,
                                                y : (760 - 338 - 67) - 415
                                            });
                                coFunctionWait(1000, [this] {});//wait
                                _uiKey->setAnimation("save");
                                count += (iconId % 10) + 1 _uiKeyText->setString(StringUtils::toString(count))
                                _uiKeyText->setAnchorPoint(ccp(0.5, 0.5));
                                //열쇠 적립창으로 날리기
                            }
                        }
                    }
                    _spinProcessQueue->next(1000);
                });
        }
    }
}

void CSlot346PiggyAdventure::onStartPickBonusGames2()
{
    //맵선택
    eff_container->addChild(_uiKey);
    isPick = true;
    if (pickRetrigger) {
        auto _type = String.fromCharCode(pickType + 96);
        changeBG(true, _type, true);
    }
    else {
        makeChoiceMap();
    }
}

void CSlot346PiggyAdventure::onStartPickBonusGames3()
{
    isClick = true;
    pickRetrigger = false;
    _spinProcessQueue->next(0);
}

void CSlot346PiggyAdventure::onEndPickBonusGames()
{
    removeInfoPig(true, true);
    changeBG(false, "normal", true);
}

void CSlot346PiggyAdventure::onEndPickBonusGames1()
{
    isPick = false;
    pickEnd = false;
    setBottomUI(true);
    m_bottomPanel->setSpinAgain();
    _spinProcessQueue->next(0);
}

void CSlot346PiggyAdventure::pickProc()
{
    auto isSuper = false;
    auto isBigwin = false;
    auto delay = 333;
    auto _featuredMap = getFeaturedMap();
    if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT) {
        if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT > 0) {
            isSuper = true;
            _tresureBox[userPick].setSuper();
        }
    }
    if (!isSuper) {
        for (auto index = 0,size = _tresureBox.size(); index < size; index++) {
            if (_tresureBox[index] && pickType > 0) {
                _tresureBox[index].setPay(pickResultInfo[StringUtils::toString(pickType)][StringUtils::toString(index)]);
                _tresureBox[index].setClick(false);
            };
        }
        if (pickResultInfo[StringUtils::toString(pickType)][StringUtils::toString(userPick)]) {
            if (pickResultInfo[StringUtils::toString(pickType)][StringUtils::toString(userPick)].indexOf("100S") >= 0) {
                isBigwin = true;
            }
            if (pickResultInfo[StringUtils::toString(pickType)][StringUtils::toString(userPick)].indexOf("50S") >= 0) {
                isBigwin = true;
            }
        }
    }
    isClick = true;
    stopEffectSound("snd_pick_count");
    _tresureBox[userPick].open();
    ArrayUtils::removeAt(remainPickInfo, userPick);
    if (isSuper) {
        playEffectSound("snd_pick_super");
        stopBGMSound();
        pickResultInfo[StringUtils::toString(pickType)] = {};
        spinType = pickType + 1;
        delay = 4000;
    }
    if (isBigwin) {
        playEffectSound("snd_pick_bigwin");
        delay = 1166;
    }
    if (isSuper || isBigwin) {
        auto action = isSuper ? "super" : "bigwin";
        if (_infoPig) {
            _infoPig->setAnimation(action);
            if (isSuper) {
                _infoPig->addAnimationComplete(action,[this](string entry) {
                        coFunctionWait(0, [this] {
                                removeInfoPig(true, true);
                            });
                    });
            }
            else {
                _infoPig->addAnimation("loop", true);
            }
        }
    }
    else {
        playEffectSound("snd_pick");
    }
    userPick = -1;
    _spinProcessQueue->next(delay);
}

void CSlot346PiggyAdventure::calcNewCoinIncreaseTimeForPick(targetAward)
{
    // 기본 0.5초
    auto retVal = 0.5;
    auto _award = targetAward || (_currentResult && _currentResult->spinResult->payout->totalAward) || 0;
    auto _multi = _award / _avgTotalBet;

    if (_multi < 5) {
        retVal = 0.5;
        // 기본 0.5초, 배수당 0.5초씩 추가
    }
    else {
        retVal = 2.5;
    }
    // 빅윈 기준이상은 빅윈팝업 플로우이므로 증가하지 않음.
    return retVal;
}

void CSlot346PiggyAdventure::onWaitPick()
{
    if (!isPick) {
        return;
    }
    if (DUG.common.settings.SLOT_HAS_TOPBAR) {
        _isAvailableClickTopbar = false;
        _topBar.checkSpinStatusUI({isRunSpin : true});
    }
    // TODO: 카운트 다운을 넣는다
    countDownTimeout = setTimeout(playCountDown.bind(this), 5000);
    countDownTimeoutSound = setTimeout(playCountDownSound.bind(this), 5000);
    _spinProcessQueue->next(0);
}

void CSlot346PiggyAdventure::makeUiKey()
{
    removeKey();
    _uiKey = _spineContainer->borrow("s346_ui_key_info");
    eff_container->addChild(_uiKey);
    _uiKey->setAnimation("appear");
    _uiKeyText = _uiKey->addBitmapText("num", {size : 46, font : "s346_num_key_collect", align : "center", alignY : "center", alignY : "center"}, eventListener);
    if (isRecovery) {
        _uiKeyText->setString(StringUtils::toString(pickCount))
        _uiKeyText->setAnchorPoint(ccp(0.5, 0.5));
    }
}

void CSlot346PiggyAdventure::playCountDownSound()
{
    clearTimeout(countDownTimeoutSound);
    playEffectSoundWithOption("snd_pick_count", "snd_pick_count");
    countDownTimeoutSound = setTimeout(playCountDownSound.bind(this), 1000);
}

void CSlot346PiggyAdventure::playCountDown()
{
    clearTimeout(countDownTimeout);
    removeCountDown();
    _countDown = _spineContainer->borrow("s346_ui_count_down");
    popup_container->addChild(_countDown);
    _countDown->setAnimation("hit");
    _countDown->addAnimationComplete("hit",[this](string entry) {
            coFunctionWait(0, [this] {
                    stopEffectSound("snd_pick_count");
                    removeCountDown();
                    userPick = remainPickInfo[CMiscUtils::getRandomIntInclusive(0, remainPickInfo.size() - 1)];
                    ArrayUtils::removeAt(remainPickInfo, userPick);
                    isClick = true;
                    if (_tresureBox[userPick]) {
                        _tresureBox[userPick].setOpen(true);
                    }
                    onSpin();
                });
        });
}

void CSlot346PiggyAdventure::makeUiScroll()
{
    removeUiScroll();
    _uiScroll = _spineContainer->borrow("s346_ui_scroll");
    eff_container->addChild(_uiScroll);
    _uiScroll->setAnimation("stay", true);
    auto position = getSlotLocalPosision(_uiScroll, "scroll_all");
    _makeButtonForScroll(_uiScroll);
}

/*
 * 지도 선택
 */
void CSlot346PiggyAdventure::makeChoiceMap()
{
    removeChoiceMap();
    _uiChoiceMap = _spineContainer->borrow("s346_ui_map");
    eff_container->addChild(_uiChoiceMap);
    if (isPick) {
        setBonustime(true);
        setAvgTotalBet();
        setOnlyBonusPotPay(false);
        m_bottomPanel->setGoodLuck();
        stopBGMSound() playBGMSound("bgm_pick_compass");
        playEffectSound("snd_intro_compass");
        coFunctionWait(2500, [this] {
                playEffectSound("snd_compass_appear");
            });
        _uiChoiceMap->setAnimation("bonus_in");
        _uiChoiceMap->addAnimation("loop", true);
        _compass = _spineContainer->borrow("s346_ui_compass");
        eff_container->addChild(_compass);
        _compass->setAnimation("in");
        _compass->addAnimationComplete("in",[this](string entry) {
                coFunctionWait(0, [this] {
                        removeUiScroll();
                        playCompassAnimation();
                    });
            });
    }
    else {
        makeInfoPig(false);
        _uiChoiceMap->setAnimation("in");
        _uiChoiceMap->addAnimationComplete("in",[this](string entry) {
                coFunctionWait(0, [this] {
                        _uiScroll.buttonMode = false;
                        _uiScroll.interactive = false;
                        isClick = false;
                        _exitButton = _spineContainer->borrow("s346_ui_out_button");
                        popup_container->addChild(_exitButton);
                        _exitButton->setAnimation("on");
                        auto position = getSlotLocalPosision(_uiChoiceMap, "num_4");
                        _makeButtonForChoiceMapExit(_exitButton);
                    });
            });
        auto _slot = [ "num_1", "num_2", "num_3", "num_5", "num_6", "num_7" ];
        for (auto index = 0; index < 3; index++) {
            auto slot = _uiChoiceMap.getSlotContainer(_slot[index]);
            auto _button = _createInteractiveRegion(0, 0, 300, 160, 0xFFFFFF, 0);
            // a,b,c
            auto type = String.fromCharCode(index + 97);
            if (_button) {
                _makeButtonForChoiceMap(_button, type);
                slot->addChild(_button);
            }
        }
        for (auto index = 0; index < 3; index++) {
            auto slot = _uiChoiceMap.getSlotContainer(_slot[index + 3]);
            _paytalbeInfo[index] = _spineContainer->borrow("s346_ui_bonus_map_info");
            _paytalbeInfo[index]->setAnimation("out", true);
            slot->addChild(_paytalbeInfo[index]);
            _paytalbeInfo[index]->setScaleY(-1);
            _makeButtonForPaytalble(_paytalbeInfo[index], index);
        }
    }
}

void CSlot346PiggyAdventure::makeInfoPig(isTresureMap)
{
    removeInfoPig();
    _infoPig = _spineContainer->borrow("s346_ui_indi_pig");
    eff_container2->addChild(_infoPig);
    if (isTresureMap) {
        if (pickRetrigger || isRecovery) {
            _infoPig->setAnimation("loop", true);
        }
        else {
            _infoPig->setAnimation("bonus_in");
            _infoPig->addAnimation("loop", true);
            playEffectSound("snd_intro_pick");
        }
    }
    else {
        playEffectSound("snd_map");
        _infoPig->setAnimation("in");
        _infoPig->addAnimation("in_loop", true);
    }
}

void CSlot346PiggyAdventure::changeBG(isTresureMap, type, out = false)
{
    isClick = true;
    playChangeBG(out ? "out" : "hit", type, isTresureMap);
    auto _type = "normal";
    switch (type) {
        case "a":
            _type = "pyramid_pick";
            break;
        case "b":
            _type = "atlantis_pick";
            break;
        case "c":
            _type = "oriental_pick";
            break;
        default:
            break;
    }
    coFunctionWait(1000, [this] {
            setChangeBG(isTresureMap, type, _type);
        });
}

void CSlot346PiggyAdventure::playChangeBG(_action, type, isTresureMap)
{
    if (_action == "hit") {
        playEffectSound("snd_plane_transition");
    }
    else {
        playEffectSound("snd_plane_transition_2");
    }
    stopBGMSound();
    auto spine = _spineContainer->borrow("s346_ui_pig_air");
    popup_container->addChild(spine);
    spine->setAnimation(_action);
    spine->addAnimationComplete(_action,[this](string entry) {
            coFunctionWait(0, [this] {
                    if (spine) {
                        spine->removeFromParent();
                        spine = nullptr;
                    }
                    _spinProcessQueue->next(0);
                });
        });
    if (isTresureMap) {
        coFunctionWait(1000, [this] {
                playBGMSound("bgm_pick_" + type);
            });
        coFunctionWait(2000, [this] {
                makeInfoPig(true);
            });
    }
}

void CSlot346PiggyAdventure::setChangeBG(isTresureMap, type, _type)
{
    removeBottomUIEffect();
    _initAllAnimations();
    if (isTresureMap) {
        if (pickRetrigger) {
            removeEndPopup();
        }
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            removeFixedSymbolForBonus();
            removeOpenBox();
            removeOpenFrame();
        }
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            replaceIconLowForPyramid();
            removeFixedSymbolForPyramid();
        }
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            replaceIconLowForOriental();
            removeFixedSymbolForOriental();
        }
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            replaceIconLowForAtlantis();
            removeFixedSymbolForAtlantis();
            removePayBoxForAtlantis();
        }
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            removeChoiceMap();
            removeCompass();
            removeFixedSymbolForBonus();
            removeOpenBox();
            removeOpenFrame();
        }
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            removeChoiceMap();
            removeCompass();
        }
        removeStay();
        spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
        setSlotMode("bonus");
        if (_uiKey) {
            _uiKey->setAnimation("loop", true);
        }
        setBonustime(true);
        setAvgTotalBet();
        // setOnlyBonusPotPay(false);
        m_bottomPanel->setGoodLuck();
        makeTreasureBox(type);
        setReelVisible(-1);
        removeUiScroll();
    }
    else {
        spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
        setSlotMode("normal");
        win_number = _incremental_bonus_pot;
        setBonustime(false);
        makeUiScroll();
        removeKey();
        _playScatterHitAnimationForNormal("stay", true);
        removeTresureBox();
        stopBGMSound();
        _changeTotalBet();
        playBGMSound("bgm_normal_reel");
    }
    bg->gotoAndStop(_type);
    reel_bg->gotoAndStop(_type);
}

void CSlot346PiggyAdventure::onScrollMouseOverListner()
{
    if (_bSpin || isClick || _bSpinAuto) {
        _uiScroll.buttonMode = false;
        return;
    }
    _uiScroll.buttonMode = true;
    _uiScroll->setAnimation("over");
}

void CSlot346PiggyAdventure::onScrollMouseOutListner()
{
    if (_bSpin || isClick || _bSpinAuto) return;
    _uiScroll->setAnimation("stay", true);
}

void CSlot346PiggyAdventure::onScrollPointerdownListner()
{
    if (_bSpin || isClick || _bSpinAuto) return;
    _uiScroll->setAnimation("open_1");
    playEffectSound("snd_button");
}

void CSlot346PiggyAdventure::onScrollPointerupListner()
{
    if (_bSpin || isClick || _bSpinAuto) return;
    openMap = true;
    isClick = true;
    _uiScroll->setAnimation("open_2");
    _uiScroll->addAnimation("stay", true);
    makeChoiceMap();
}

void CSlot346PiggyAdventure::onChoiceMapMouseOverListner(type)
{
    if (_bSpin || isClick) return;
    _uiChoiceMap->setAnimation("bonus_" + type + "_over");
}

void CSlot346PiggyAdventure::onChoiceMapMouseOutListner()
{
    if (_bSpin || isClick) return;
    _uiChoiceMap->setAnimation("info_on");
}

void CSlot346PiggyAdventure::onChoiceMapPointerupListner(type)
{
    if (_bSpin || isClick) return;
    isClick = true;
    _uiChoiceMap->setAnimation("bonus_" + type + "_info_up");
    coFunctionWait(500, [this] {
            removeUiTresureMap();
            _uiTresureMap = _spineContainer->borrow("s346_ui_bonus_" + type + "_map");
            eff_container->addChild(_uiTresureMap);
            makeTreasureBox(type, true);
            if (_exitButton) {
                _makeButtonForTresureMapExit(_exitButton);
            }
            if (_uiChoiceMap) {
                _uiChoiceMap->setVisible(false);
            }
            if (_infoPig) {
                _infoPig->setAnimation("in_loop2", true)
            }
            isClick = false;
        });
}

void CSlot346PiggyAdventure::onChoiceMapPointerdownListner(type)
{
    if (_bSpin || isClick) return;
    // isClick = true;
    playEffectSound("snd_button");
    _uiChoiceMap->setAnimation("bonus_" + type + "_info_down");
}

void CSlot346PiggyAdventure::onPaytableMouseOverListner(index)
{
    if (_bSpin || isClick) return;
    _paytalbeInfo[index]->setAnimation("over");
}

void CSlot346PiggyAdventure::onPaytableMouseOutListner(index)
{
    if (_bSpin || isClick) return;
    _paytalbeInfo[index]->setAnimation("on");
}

void CSlot346PiggyAdventure::onPaytablePointerdownListner()
{
    if (_bSpin || isClick) return;
    playEffectSound("snd_button");
}

void CSlot346PiggyAdventure::onPaytablePointerupListner(index)
{
    if (_bSpin || isClick) return;
    _paytalbeInfo[index]->setAnimation("hit");
    auto page = 0;
    switch (index) {
        case 0:
            page = 3;
            break;
        case 1:
            page = 2;
            break;
        case 2:
            page = 4;
            break;
        default:
            break;
    }
    if (_paytable) {
        if (_paytable.visible) {
            _paytable.hide();
        }
        else {
            _paytable.show();
            addChild(_paytable);
            _paytable.paytable_current_index = page _paytable._main.paytable->gotoAndStop(page);
        }
    }
    else {
        auto _url = getPaytableResUrl();
        auto _paytableResource = {
            name : StringUtils.formatExt(DUG.common.settings.SLOT_PAYTABLE_CLASS_HOLDER_FORMAT, {slot_type : _slotConfig.slot_type}),
            url : _url,
            metadata : {
                path : "relative",
                type : "animate",
                slot_type : _slotConfig.slot_type,
                version : _slotConfig.version,
            }
        };
        auto _simpleLoader = CResourceSimpleLoader.getInstance();
        _simpleLoader.clear();
        _simpleLoader.addResource(_paytableResource);
        _simpleLoader.load();
        eventListener.on(
            _simpleLoader, "complete", _onCompleteLoadOpenBigWinPopupBindMe = function() {
                eventListener.off(_simpleLoader, "complete", _onCompleteLoadOpenBigWinPopupBindMe);
                _createPaytable();
                // 페이테이블 아래로 이벤트가 전이되지 않도록
                _paytable.interactive = true;
                _paytable->setPositionY(y + _slotContainer->getPositionY() * _slotContainer.scale._y);
                _paytable->setPositionX(x + _slotContainer->getPositionX() * _slotContainer.scale._x);

                if (_layouts.paytable && _layouts.paytable.adjust_x) {
                    _paytable->getPositionX() += _layouts.paytable.adjust_x;
                }
                if (_layouts.paytable && _layouts.paytable.adjust_y) {
                    _paytable->getPositionY() += _layouts.paytable.adjust_y;
                }

                _paytable.scale._x = _slotContainer.scale._x;
                _paytable.scale._y = _slotContainer.scale._y;
                // _slotContainer->addChild(_paytable);
                addChild(_paytable);
                eventListener.on(
                    _paytable, "initialized", onInitializedPaytableBindMe = function() {
                        eventListener.off(_paytable, "initialized", onInitializedPaytableBindMe);
                        _paytable.setPaytablePayout(_unitBet);
                        _paytable.paytable_current_index = page _paytable._main.paytable->gotoAndStop(page);
                    }.bind(this, page));
            }.bind(this, page));
    }
}

void CSlot346PiggyAdventure::onExitMouseOverListner()
{
    if (_bSpin || isClick) return;
    _exitButton->setAnimation("over");
}

void CSlot346PiggyAdventure::onExitMouseOutListner()
{
    if (_bSpin || isClick) return;
    _exitButton->setAnimation("on");
}

void CSlot346PiggyAdventure::onExitPointerdownListner()
{
    if (_bSpin || isClick) return;
    playEffectSound("snd_button");
    _exitButton->setAnimation("hit_1");
}

void CSlot346PiggyAdventure::onExitForChoiceMapPointerupListner()
{
    if (_bSpin || isClick) return;
    removePaytableInfo();
    isClick = true;
    _exitButton->setAnimation("hit_2");
    _exitButton->addAnimationComplete("hit_2",[this](string entry) {
            coFunctionWait(0, [this] {
                    removeExitButton();
                    _uiChoiceMap->setAnimation("out");
                    _uiChoiceMap->addAnimationComplete("out",[this](string entry) {
                            coFunctionWait(10, [this] {
                                    removeChoiceMap();
                                    _uiScroll.buttonMode = true;
                                    _uiScroll.interactive = true;
                                    isClick = false;
                                    openMap = false;
                                });
                        });
                    removeInfoPig(true, false);
                });
        });
}

void CSlot346PiggyAdventure::onExitForTresureMapPointerupListner()
{
    if (_bSpin || isClick) return;
    isClick = true;
    _exitButton->setAnimation("hit_2");
    _exitButton->addAnimationComplete("hit_2",[this](string entry) {
            coFunctionWait(0, [this] {
                    if (_exitButton) {
                        _makeButtonForChoiceMapExit(_exitButton);
                    }
                    if (_uiChoiceMap) {
                        _uiChoiceMap->setVisible(true);
                        _uiChoiceMap->setAnimation("info_on");
                    }
                    if (_infoPig) {
                        _infoPig->setAnimation("in_loop", true);
                    }
                    removeTresureBox();
                    removeUiTresureMap();
                    isClick = false;
                });
        });
}

/**
 * 버튼 ui 생성
 * @protected
 */
void CSlot346PiggyAdventure::_makeButtonForScroll(_button)
{
    _button.buttonMode = true;
    _button.interactive = true;
    _button.mouseover = onScrollMouseOverListner.bind(this);
    _button.mouseout = onScrollMouseOutListner.bind(this);
    _button.mousedown = onScrollPointerdownListner.bind(this);
    _button.mouseup = onScrollPointerupListner.bind(this);
}

/**
 * 버튼 ui 생성
 * @protected
 */
void CSlot346PiggyAdventure::_makeButtonForChoiceMapExit(_button)
{
    _button.buttonMode = true;
    _button.interactive = true;
    _button.mouseover = onExitMouseOverListner.bind(this);
    _button.mouseout = onExitMouseOutListner.bind(this);
    _button.mousedown = onExitPointerdownListner.bind(this);
    _button.mouseup = onExitForChoiceMapPointerupListner.bind(this);
}

/**
 * 버튼 ui 생성
 * @protected
 */
void CSlot346PiggyAdventure::_makeButtonForChoiceMap(_button, type)
{
    _button.buttonMode = true;
    _button.interactive = true;
    _button.mouseover = onChoiceMapMouseOverListner.bind(this, type);
    _button.mouseout = onChoiceMapMouseOutListner.bind(this);
    _button.mouseup = onChoiceMapPointerupListner.bind(this, type);
    _button.mousedown = onChoiceMapPointerdownListner.bind(this, type);
}

/**
 * 버튼 ui 생성
 * @protected
 */
void CSlot346PiggyAdventure::_makeButtonForPaytalble(_button, index)
{
    _button.buttonMode = true;
    _button.interactive = true;
    _button.mouseover = onPaytableMouseOverListner.bind(this, index);
    _button.mouseout = onPaytableMouseOutListner.bind(this, index);
    _button.mouseup = onPaytablePointerupListner.bind(this, index);
    _button.mousedwon = onPaytablePointerdownListner.bind(this, index);
}

/**
 * 버튼 ui 생성
 * @protected
 */
void CSlot346PiggyAdventure::_makeButtonForTresureMapExit(_button)
{
    _button.buttonMode = true;
    _button.interactive = true;
    _button.mouseover = onExitMouseOverListner.bind(this);
    _button.mouseout = onExitMouseOutListner.bind(this);
    _button.mousedown = onExitPointerdownListner.bind(this);
    _button.mouseup = onExitForTresureMapPointerupListner.bind(this);
}

void CSlot346PiggyAdventure::playCompassAnimation()
{
    auto _type = String.fromCharCode(pickType + 96);
    playEffectSound("snd_compass_spin");
    _compass->setAnimation("bonus_" + _type);
    _compass->addAnimationComplete("bonus_" + _type,[this](string entry) {
            coFunctionWait(0, [this] {
                    playEffectSound("snd_compass_stop");
                    _uiChoiceMap->setAnimation("bonus_" + _type);
                    coFunctionWait(700, [this] {});//wait
                    changeBG(true, _type);
                });
        });
}

void CSlot346PiggyAdventure::createTresureBox(options)
{
    return new CSlot346PiggyAdventureTresureBox(options);
}

/*
 * 보물상자
 */
void CSlot346PiggyAdventure::makeTreasureBox(_type, _info = false)
{
    removeTresureBox();
    _tresureBox = create1DArray(30, null);
    remainPickInfo = [];
    auto pickIndex = _type.charCodeAt(0) - 97;
    _uiTresureBox = _spineContainer->borrow("s346_ui_box_set");
    _uiTresureBox->setAnimation("on");
    eff_container->addChild(_uiTresureBox);
    if (_info) {
        _uiTresureBox->setScaleX(0.9);
        _uiTresureBox->setScaleY(0.9);
    }
    for (auto index = 0; index < 30; index++) {
        auto _pay = pickResultInfo[StringUtils::toString(pickIndex + 1)][StringUtils::toString(index)];
        auto _slot = _uiTresureBox.getSlotContainer(StringUtils::format("num_%d", index + 1 ));
        if (!_pay || _pay == undefined || _pay.size() == 0) {
            remainPickInfo.push_back(index);
        }
        _tresureBox[index] = createTresureBox({
            type : _type,
            isPick : isPick ? true : false,
            pay : (_pay && _pay.size() > 0) ? _pay : "",
            index : index,
            slot : this,
            container : _slot,
            _spineContainer : _spineContainer
        });
        _tresureBox[index].initBox();
    }
}

void CSlot346PiggyAdventure::removePaytableInfo()
{
    for (auto index = 0,size = _paytalbeInfo.size(); index < size; index++) {
        if (_paytalbeInfo[index]) {
            _paytalbeInfo[index]->removeFromParent();
            _paytalbeInfo[index] = nullptr;
        }
    }
}

void CSlot346PiggyAdventure::removeUiTresureMap()
{
    if (_uiTresureMap) {
        _uiTresureMap->removeFromParent();
        _uiTresureMap = nullptr;
    }
}

void CSlot346PiggyAdventure::removeInfoPig(animation = false, isTresureMap = false)
{
    if (animation) {
        if (isTresureMap) {
            _infoPig->setAnimation("out");
            _infoPig->addAnimationComplete("out",[this](string entry) {
                    coFunctionWait(0, [this] {
                            removeInfoPig();
                        });
                });
        }
        else {
            _infoPig->setAnimation("in_out");
            _infoPig->addAnimationComplete("in_out",[this](string entry) {
                    coFunctionWait(0, [this] {
                            removeInfoPig();
                        });
                });
        }
    }
    else {
        if (_infoPig) {
            _infoPig->removeFromParent();
            _infoPig = nullptr;
        }
    }
}

void CSlot346PiggyAdventure::removeTresureBox()
{
    for (auto index = 0,size = _tresureBox.size(); index < size; index++) {
        if (_tresureBox[index]) {
            _tresureBox[index]->removeFromParent();
            _tresureBox[index] = nullptr;
        }
    }
    if (_uiTresureBox) {
        _uiTresureBox->removeFromParent();
        _uiTresureBox = nullptr;
    }
}

void CSlot346PiggyAdventure::removeCountDown()
{
    if (_countDown) {
        _countDown->removeFromParent();
        _countDown = nullptr;
    }
}

void CSlot346PiggyAdventure::removeUiScroll()
{
    if (_uiScroll) {
        _uiScroll->removeFromParent();
        _uiScroll = nullptr;
    }
}

void CSlot346PiggyAdventure::removeExitButton()
{
    if (_exitButton) {
        _exitButton->removeFromParent();
        _exitButton = nullptr;
    }
}

void CSlot346PiggyAdventure::removeCompass()
{
    if (_compass) {
        _compass->removeFromParent();
        _compass = nullptr;
    }
}

void CSlot346PiggyAdventure::removeChoiceMap()
{
    if (_uiChoiceMap) {
        _uiChoiceMap->removeFromParent();
        _uiChoiceMap = nullptr;
    }
}

void CSlot346PiggyAdventure::removeKey()
{
    if (_uiKey) {
        _uiKey->removeFromParent();
        _uiKey = nullptr;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 함수
////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::onStartSuperFreespin()
{
    superFreespinStart = false;
    isSuperFreespin = true;
    isPick = false;
    pickEnd = false;
    _incremental_bonus_pot = 0;
    _incremental_bonus_pot_end = 0;
    bonuspot_number = 0;
    orientalTotalAward = 0;
    orientalTotalAwardPre = 0;
    _changeTotalBet();
    setOnlyBonusPotPay(false);
    setAvgTotalBet();
    m_bottomPanel->setGoodLuck();
    _spinProcessQueue->next(100);
}

void CSlot346PiggyAdventure::onStartSuperFreespin1()
{
    playSuperStart();
}

void CSlot346PiggyAdventure::onStartSuperFreespin2()
{
    firstSpinForSuper = true;
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        _payBoxNumForAtlantis = _spineContainer->borrow("s346_ui_bonus_b_pay");
        eff_container3->addChild(_payBoxNumForAtlantis);
        _payBoxNumForAtlantisBig = _spineContainer->borrow("s346_ui_bonus_b_pay");
        eff_container3->addChild(_payBoxNumForAtlantisBig);
        _payBoxTextForAtlantis = create1DArray(paylinesForAtlantis.size(), null);
        _payBoxTextForAtlantisBig = create1DArray(paylinesForAtlantis.size(), null);
        _payHitAniForAtlantis = create1DArray(paylinesForAtlantis.size(), null);
        linePayForAtlantis = create1DArray(paylinesForAtlantis.size(), 0);
        superFreespinStartAnimationForAtlantis();
        _spinProcessQueue->next(2500);
    }
    else {
        _spinProcessQueue->next(100);
    }
}

void CSlot346PiggyAdventure::onEndSuperFreespin()
{
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        if (_payoutSpine) {
            _payoutSpine->setAnimation("out");
            _payoutSpine->addAnimationComplete("out",[this](string entry) {
                    coFunctionWait(0, [this] {
                            removeStay();
                            removePayoutSpine();
                            playEndPopup();
                        });
                });
        }
    }
    else {
        playEndPopup();
    }
}

void CSlot346PiggyAdventure::onEndSuperFreespin1()
{
    _spinProcessQueue->next(0);
}

void CSlot346PiggyAdventure::onEndSuperFreespin2()
{
    isSuperFreespin = false;
    superFreespinEnd = false;
    _writeSlotActionLog({
        category_info : "",
        action : "SUPER_BONUS_EXIT",
        action_info : "",
        isplaynow : 1
    });
    if (pickCount > 0) {
        _spinProcessQueue->next(0);
    }
    else {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            setNormal();
            m_bottomPanel->setSpinAgain();
            superTotalAward = 0;
        }
        _spinProcessQueue->next(100);
    }
}

void CSlot346PiggyAdventure::setNormal()
{
    _initAllAnimations();
    removeEndPopup();
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        removeFixedSymbolForBonus();
        removeOpenBox();
        removeOpenFrame();
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        replaceIconLowForPyramid();
        removeFixedSymbolForPyramid();
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        replaceIconLowForOriental();
        removeFixedSymbolForOriental();
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        replaceIconLowForAtlantis();
        removeFixedSymbolForAtlantis();
        removePayBoxForAtlantis();
    }
    removeStay();
    removeBottomUIEffect();
    spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
    setBottomUI(true);
    setSlotMode("normal");
    _playScatterHitAnimationForNormal("stay", true);
    removeKey();
    makeUiScroll();
    stopBGMSound();
    _changeTotalBet();
    playBGMSound("bgm_normal_reel");
}

void CSlot346PiggyAdventure::playEndPopup()
{
    removeEndPopup();
    stopBGMSound();
    auto _type = String.fromCharCode(pickType + 96);
    playEffectSound("snd_result_" + _type);
    auto totalAward = totalAward(true);
    _endPopup = _spineContainer->borrow("s346_ui_bonus_" + _type + "_closing");
    _endPopup->setAnimation("hit");
    popup_container->addChild(_endPopup);
    auto _text = _endPopup->addBitmapText("num", {size : 74, font : "s346_num_congratulations", align : "center", alignY : "center", alignY : "center"}, eventListener);
    _text->setString(CMiscUtils::moneyFormatter(totalAward))
    _text->setAnchorPoint(ccp(0.5, 0.5));
    _endPopup->addAnimationComplete("hit",[this](string entry) {
            coFunctionWait(0, [this] {
                    _spinProcessQueue->next(0);
                });
        });
    playEffectSpine("s346_ui_payout", popup_container, "hit");
}

void CSlot346PiggyAdventure::getMultipotForSuper(pay)
{
    // 500 , 300, 150, 100, 50
    if (pay == 500) {
        return 5;
    }
    if (pay == 300) {
        return 4;
    }
    if (pay == 150) {
        return 3;
    }
    if (pay == 100) {
        return 2;
    }
    if (pay == 50) {
        return 1;
    }
}

void CSlot346PiggyAdventure::playSuperStart()
{
    removeStay();
    stopBGMSound();
    playBGMSound("bgm_super_" + String.fromCharCode(pickType + 96));
    switch (pickType) {
        case 1:
            playSlotStayForPyramid();
            break;
        case 2:
            playSlotStayForAtlantis();
            break;
        case 3:
            playSlotStayForOriental();
            break;
        default:
            break;
    }
}

void CSlot346PiggyAdventure::playSlotStayForPyramid()
{
    auto delay = 666;
    auto action = "appear";
    playEffectSound("snd_intro_super_a");
    _staySpine = _spineContainer->borrow("s346_ui_bonus_a_pig");
    _staySpine->setAnimation(action);
    _staySpine->addAnimation("loop", true);
    eff_container2->addChild(_staySpine);
    coFunctionWait(delay, [this] {
            playSuperInPopupForPyramid();
        });
}

void CSlot346PiggyAdventure::playSuperInPopupForPyramid()
{
    setSuperIn("a");
    removeOpenPopup();
    playBlackLayer("in", black);
    playEffectSpine("s346_ui_bonus_a_reel_eff", eff_container, "hit");
    _openPopup = _spineContainer->borrow("s346_ui_bonus_a_bonus");
    _openPopup->setAnimation("hit");
    popup_container->addChild(_openPopup);
    _openPopup->addAnimationComplete("hit",[this](string entry) {
            coFunctionWait(0, [this] {
                    playEffectSound("snd_pig_a_1");
                    removeOpenPopup();
                    removeBlackLayer(true);
                    _spinProcessQueue->next(0);
                });
        });
}

void CSlot346PiggyAdventure::playSlotStayForAtlantis()
{
    auto delay = 1000;
    auto action = "appear";
    playEffectSound("snd_intro_super_b");
    _staySpine = _spineContainer->borrow("s346_ui_bonus_b_pig");
    _staySpine->setAnimation(action);
    eff_container2->addChild(_staySpine);
    coFunctionWait(delay, [this] {
            playSuperInPopupForAtlantis();
        });
}

void CSlot346PiggyAdventure::playSuperInPopupForAtlantis()
{
    replaceIconLowForAtlantis();
    setSuperIn("b");
    removeOpenPopup();
    _openPopup = _spineContainer->borrow("s346_ui_bonus_b_bonus");
    _openPopup->setAnimation("hit");
    popup_container->addChild(_openPopup);
    _openPopup->addAnimationComplete("hit",[this](string entry) {
            coFunctionWait(0, [this] {
                    playPayoutSpineForAtlantis();
                    removeOpenPopup();
                    _spinProcessQueue->next(0);
                });
        });
}

void CSlot346PiggyAdventure::playPayoutSpineForAtlantis()
{
    _payoutSpine = _spineContainer->borrow("s346_ui_bonus_b_pearl");
    eff_container->addChild(_payoutSpine);
    if (isRecovery) {
        _payoutSpine->setAnimation("stay", true);
    }
    else {
        _payoutSpine->setAnimation("appear");
        _payoutSpine->addAnimation("stay", true);
    }
}

void CSlot346PiggyAdventure::playSlotStayForOriental()
{
    replaceIconLowForOriental();
    auto delay = 630;
    auto action = "in";
    playEffectSound("snd_intro_super_c");
    _staySpine = _spineContainer->borrow("s346_ui_bonus_c_pig");
    _staySpine->setAnimation(action);
    eff_container->addChild(_staySpine);
    coFunctionWait(delay, [this] {
            playSuperInPopupForOriental();
        });
    _staySpine->addAnimation("side_in");
    _staySpine->addAnimation("side_loop", true);
    coFunctionWait(4000, [this] {
            playEffectSpine("s346_ui_bonus_c_reel_eff", eff_container, "start");
        });
}

void CSlot346PiggyAdventure::playSuperInPopupForOriental()
{
    playBlackLayer("in", black);
    setSuperIn("c");
    removeOpenPopup();
    _openPopup = _spineContainer->borrow("s346_ui_bonus_c_bonus");
    _openPopup->setAnimation("hit");
    eff_container2->addChild(_openPopup);
    _openPopup->addAnimationComplete("hit",[this](string entry) {
            coFunctionWait(0, [this] {
                    removeOpenPopup();
                    removeBlackLayer(true);
                    _spinProcessQueue->next(0);
                });
        });
}

void CSlot346PiggyAdventure::setSuperIn(type)
{
    _setResultIconsAlpha(1);
    _releaseAnimationIcons();
    hideAllPaylines();
    setBonustime(false);
    setSlotMode("bonus");
    setResultIconVisible();
    removeTresureBox();
    setOnlyBonusPotPay(false);
    if (pickCount == 0) {
        removeKey();
    }
    else {
        if (_uiKey) {
            _uiKey->setAnimation("bonus_loop", true);
        }
    }
    m_bottomPanel->setGoodLuck();
    auto _type = "";
    switch (type) {
        case "a":
            _type = "pyramid_super";
            superFreespinTotalCount = 7;
            break;
        case "b":
            _type = "atlantis_super";
            superFreespinTotalCount = 10;
            break;
        case "c":
            _type = "oriental_super";
            superFreespinTotalCount = 5;
            break;
        default:
            break;
    }
    writeAcionLogForSuper();
    setFreespinLeftcount(superFreespinRemainCount, superFreespinTotalCount);
    bg->gotoAndStop(_type);
    reel_bg->gotoAndStop(_type);
}

void CSlot346PiggyAdventure::writeAcionLogForSuper()
{
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        _writeSlotActionLog({
            category_info : "",
            action : "SUPER_BONUS_ENTER: " + superFreespinTotalCount,
            action_info : "1-king tut",
            isplaynow : 1
        });
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        _writeSlotActionLog({
            category_info : "",
            action : "SUPER_BONUS_ENTER: " + superFreespinTotalCount,
            action_info : "2-neptune",
            isplaynow : 1
        });
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        _writeSlotActionLog({
            category_info : "",
            action : "SUPER_BONUS_ENTER: " + superFreespinTotalCount,
            action_info : "3-ancient",
            isplaynow : 1
        });
    }
}

void CSlot346PiggyAdventure::removeStay()
{
    if (_staySpine) {
        _staySpine->removeFromParent();
        _staySpine = nullptr;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 피라미드 함수
////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::_createSlotReelWrapperForPyramid(instance, options)
{
    options.onBounceStopSpin = _onBounceStopSpinForPyramid.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    auto randomStart = CMiscUtils::getRandomIntInclusive(0, options.reelArray.size() - 1);
    options.startPosition = randomStart;
    options.spineContainer = _spineContainer;
    options.findIndexFunc = findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventurePyramidReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
void CSlot346PiggyAdventure::_onBounceStopSpinForPyramid(_slotReelWrapper)
{
    const reelIndex = _slotReelWrapper->reelIndex;
    // 릴 스핀 종료 재생
    auto _resultIcons = _slotReelWrapper->getResultIcons();
    auto scatterAppear = false;
    for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
        if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
            continue;
        }
        auto _icon = _resultIcons[rowIndex];
        auto _iconId = _icon->_dug_icon_id;
        if (!isPyramidScatter(_iconId)) {
            continue;
        }
        scatterAppear = true;
        _icon->setVisible(false);
        auto spineName = "s346_icon_bonus_a_high";
        if (_iconId == 210) {
            spineName = "s346_icon_bonus_a_wild";
        }
        auto action = "lock";
        if (_iconId == 210 || (_iconId == 203 && reelIndex == 0)) {
            action = "appear";
        }
        _fixedSymbolForPyramid[reelIndex][rowIndex] = _spineContainer->borrow(spineName);
        layer_reel_frame_pyramid->addChild(_fixedSymbolForPyramid[reelIndex][rowIndex]);
        _fixedSymbolForPyramid[reelIndex][rowIndex]->setAnimation(action);
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
        _fixedSymbolForPyramid[reelIndex][rowIndex]->adjustPosition(_reel, rowIndex, 1);
        _fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id = _iconId;
        _fixedSymbolForPyramid[reelIndex][rowIndex]._reelIndex = reelIndex;
        _fixedSymbolForPyramid[reelIndex][rowIndex]._rowIndex = rowIndex;
        if (reelIndex != 0) {
            setIconTextForPyramid(_fixedSymbolForPyramid[reelIndex][rowIndex], _iconId);
            if (action == "lock" && _fixedSymbolForPyramid[reelIndex][rowIndex]._attach) {
                _fixedSymbolForPyramid[reelIndex][rowIndex]._attach->setAnimation(action);
            }
        }
    }
    playEffectSound("snd_reel_stop");
    if (scatterAppear) {
        playEffectSound("snd_symbol_appear_a");
    }
}

void CSlot346PiggyAdventure::pyramidProc()
{
    updateStaySymbolForPyramid();
    _spinProcessQueue->next(300);
}

void CSlot346PiggyAdventure::pyramidPayoutProc()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                auto _icon = _fixedSymbolForPyramid[reelIndex][rowIndex];
                auto action = "on";
                _icon->setAnimation(action);
                if (_icon->_attach) {
                    _icon->_attach->setAnimation(action);
                }
            }
        }
    }
    setOnlyBonusPotPay(true);
    auto _totalAward = _currentResult->spinResult->payout->totalAward;
    auto kind = getPayoutBeforeEffectKind(_totalAward / _avgTotalBet);
    if (kind != "none") {
        m_bottomPanel->playPayoutWinEffect(kind);
        playPayoutBeforeEffectSound(kind);
    }
    _spinProcessQueue->next(1000);
}

void CSlot346PiggyAdventure::pyramidPayoutProc1()
{
    if (_currentResult->spinResult->payout->lines) {
        totalPayTemp = 0;
        _staySpine->setAnimation("hit", true);
        stopBGMSound();
        payoutType = "a1";
        playBGMSound("bgm_payout_a1");
        coFunctionWait(0, [this]{
                auto _hitIcons = _findAllHitIconsForPyramid();
                auto totalPay = 0;
                for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
                    auto _slotReelWrapper = allReels[spinType][reelIndex];
                    auto icons = _slotReelWrapper->getResultIcons();
                    for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                        if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                            if (!ArrayUtils::contains(_hitIcons, _fixedSymbolForPyramid[reelIndex][rowIndex])) {
                                _fixedSymbolForPyramid[reelIndex][rowIndex]->setOpacity(255 * 0.2);
                                _fixedSymbolForPyramid[reelIndex][rowIndex]->setAnimation("on");
                                icons[rowIndex]->setVisible(false);
                            }
                        }
                    }
                }
                for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
                    auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
                    for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                        if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                            if (!ArrayUtils::contains(_hitIcons, _fixedSymbolForPyramid[reelIndex][rowIndex])) {
                                continue;
                            }
                            auto iconId = _fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id;
                            if (iconId == 203) {
                                auto delay = 500;
                                auto action = "pay";
                                if (fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
                                    action = "multipot" delay = 3300;
                                }
                                _fixedSymbolForPyramid[reelIndex][rowIndex].parent->addChild(_fixedSymbolForPyramid[reelIndex][rowIndex]);
                                _fixedSymbolForPyramid[reelIndex][rowIndex]->setAnimation(action);
                                _fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id = iconId;
                                _fixedSymbolForPyramid[reelIndex][rowIndex]._reelIndex = reelIndex;
                                _fixedSymbolForPyramid[reelIndex][rowIndex]._rowIndex = rowIndex;
                                if (_fixedSymbolForPyramid[reelIndex][rowIndex]._attach) {
                                    _fixedSymbolForPyramid[reelIndex][rowIndex]._attach->setAnimation(action);
                                }
                                playPayoutEffectIconForPyramid(reelIndex, rowIndex);
                                coFunctionWait(delay, [this] {});//wait
                            }
                        }
                    }
                }
                stopEffectSound("snd_payout_" + payoutType);
                playEffectSound("snd_payout_" + payoutType + "_end");
                _incremental_bonus_pot_end = 0;
                _incremental_bonus_pot = totalPayTemp;
                bonuspot_number = totalPayTemp;
                _spinProcessQueue->next(2000);
            });
    }
}

void CSlot346PiggyAdventure::pyramidPayoutProc2()
{
    removeMultipotPopup();
    if (_currentResult->spinResult->payout->lines) {
        showHitPaylines();
        playHidePaylineProcess();
        auto _hitIcons = _findAllHitIconsForPyramid();
        for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
            for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                    auto _icon = _fixedSymbolForPyramid[reelIndex][rowIndex];
                    if (!ArrayUtils::contains(_hitIcons, _icon)) {
                        continue;
                    }
                    auto action = "hit";
                    _icon->setAnimation(action, true);
                    if (_icon->_attach) {
                        _icon->_attach->setAnimation(action, true);
                    }
                }
            }
        }
        auto _init = totalPayTemp;
        auto _totalAward = _currentResult->spinResult->payout->totalAward;
        auto _duration = calcNewCoinIncreaseTime(_totalAward - _init);
        payoutType = "a2";
        playEffectSoundWithOption("snd_payout_" + payoutType, "snd_payout_" + payoutType, true);
        playPayoutIncrement({
            delay : 2000,
            duration : _duration,
            init : _init,
            result : _totalAward,
            payoutType : "update_bonuspot"
        });
    }
}

void CSlot346PiggyAdventure::playPayoutEffectIconForPyramid(reelIndex, rowIndex)
{
    coFunctionWait(0, [this]{
            auto delay = 300;
            if (fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
                delay = 500;
                coFunctionWait(500, [this] {});//wait
                playEffectSound("snd_text_popup_a");
            }
            else {
                playEffectSound("snd_count_a");
            }
            coFunctionWait(delay, [this] {});//wait
            totalPayTemp += fixedPayoutForSuper[reelIndex][rowIndex] * _avgTotalBet / 10;
            bonuspot_number = totalPayTemp;
            if (fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
                playMultipotPopup(fixedPayoutForSuper[reelIndex][rowIndex], true);
            }
            if (m_bottomPanel) {
                m_bottomPanel->playPayoutEffect();
            }
        });
}

void CSlot346PiggyAdventure::playHidePaylineProcess()
{
    _hidePaylineProcessQueue.killAndDone();
    _hidePaylineProcessQueue.add(
        hidePaylineBindMe
            ? hidePaylineBindMe
            : (hidePaylineBindMe =
                   hideAllPaylines.bind(this)));
    auto delay = 1000;
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        delay = 2000;
    }
    _hidePaylineProcessQueue.next(delay);
}

void CSlot346PiggyAdventure::calcCoinIncreaseTimeForPyramid()
{
    auto _duration = 0;
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                auto _icon = _fixedSymbolForPyramid[reelIndex][rowIndex];
                auto iconId = _icon->_dug_icon_id;
                if (iconId == 203) {
                    if (fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
                        _duration += 2;
                    }
                    else {
                        _duration += 0.5;
                    }
                }
            }
        }
    }
    return _duration + 3;
}

void CSlot346PiggyAdventure::updateStaySymbolForPyramid()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
                auto _icon = _fixedSymbolForPyramid[reelIndex][rowIndex];
                _icon->setAnimation("loop", true);
                layer_reel_frame_pyramid->addChild(_icon);
                _icon->adjustPosition(_reel, rowIndex, 1);
                setIconTextForPyramid(_icon, _icon->_dug_icon_id);
            }
        }
    }
}

void CSlot346PiggyAdventure::recoveryProcForPyramid()
{
    getFeaturedInfo();
    _staySpine = _spineContainer->borrow("s346_ui_bonus_a_pig");
    _staySpine->setAnimation("loop", true);
    eff_container2->addChild(_staySpine);
    replaceIconLowForPyramid();
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            auto _iconId = fixedPlaylineForSuper[reelIndex][rowIndex];
            if (!isPyramidScatter(_iconId)) {
                continue;
            }
            _icon->setVisible(false);
            auto spineName = "s346_icon_bonus_a_high";
            if (_iconId == 210) {
                spineName = "s346_icon_bonus_a_wild";
            }
            _fixedSymbolForPyramid[reelIndex][rowIndex] = _spineContainer->borrow(spineName);
            layer_reel_frame_pyramid->addChild(_fixedSymbolForPyramid[reelIndex][rowIndex]);
            _fixedSymbolForPyramid[reelIndex][rowIndex]->adjustPosition(_reel, rowIndex, 1);
            _fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id = _iconId;
            _fixedSymbolForPyramid[reelIndex][rowIndex]._reelIndex = reelIndex;
            _fixedSymbolForPyramid[reelIndex][rowIndex]._rowIndex = rowIndex;
            setIconTextForPyramid(_fixedSymbolForPyramid[reelIndex][rowIndex], _iconId);
        }
    }
    updateStaySymbolForPyramid();
}

/*
 * 피라미드 아이콘 텍스트
 */
void CSlot346PiggyAdventure::setIconTextForPyramid(_icon, _iconId, random = false, on = false)
{
    if (_icon == undefined || _iconId != 203) {
        return;
    }
    if (_icon->_attach) {
        _icon->_attach.removeLinkSpine();
        _icon->_attach->removeFromParent();
        _icon->_attach = nullptr;
    }
    auto isMultipot = false;
    auto reelIndex = _icon->_reelIndex;
    auto rowIndex = _icon->_rowIndex;
    auto fixPay = 0;
    auto type = 0;
    if (random) {
        auto pay = [ 500, 300, 150, 100, 50, 30, 20, 15, 12, 10, 5, 3, 2, 1 ];
        auto _pay = pay[CMiscUtils::getRandomIntInclusive(0, pay.size() - 1)];
        if (_pay >= 50) {
            isMultipot = true;
        }
        fixPay = _avgTotalBet * _pay / 10;
        type = getMultipotForSuper(_pay);
    }
    else {
        fixPay = _avgTotalBet * fixedPayoutForSuper[reelIndex][rowIndex] / 10;
        if (fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
            isMultipot = true;
        }
        type = getMultipotForSuper(fixedPayoutForSuper[reelIndex][rowIndex]);
    }
    auto spineNameBox = "s346_icon_pay_box";
    if (on) {
        spineNameBox += "_on";
    }
    auto payBox = _spineContainer->borrow(spineNameBox);
    if (!isMultipot) {
        auto _text = payBox->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
        _text->setString(CMiscUtils::MakeUnitString(fixPay))
        _text->setAnchorPoint(ccp(0.5, 0.5));
        _icon->addChild(payBox);
        _icon->_attach = payBox;
    }
    else {
        auto spineNameMulti = "s346_icon_bonus_a_" + multipots[type];
        if (on) {
            spineNameMulti += "_on";
        }
        auto _multipot = _spineContainer->borrow(spineNameMulti);
        _multipot->setAnimation("on");
        payBox.addLinkSpine(_multipot, "num2");
        _icon->addChild(payBox);
        _icon->_attach = payBox;
    }
}

void CSlot346PiggyAdventure::superFreespinStartAnimationForPyramid()
{
    coFunctionWait(0, [this]{
            playEffectSound("snd_1st_reel_a");
            auto _slotReelWrapper = allReels[spinType][0];
            auto _resultIcons = _slotReelWrapper->getResultIcons();
            for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
                auto spineName = "s346_icon_bonus_a_high";
                _fixedSymbolForPyramid[0][rowIndex] = _spineContainer->borrow(spineName);
                layer_reel_frame_pyramid->addChild(_fixedSymbolForPyramid[0][rowIndex]);
                auto _reel = _getTypeReel(0, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
                _fixedSymbolForPyramid[0][rowIndex]->adjustPosition(_reel, rowIndex, 1);
                _fixedSymbolForPyramid[0][rowIndex]._dug_icon_id = 203;
                _fixedSymbolForPyramid[0][rowIndex]._reelIndex = 0;
                _fixedSymbolForPyramid[0][rowIndex]._rowIndex = rowIndex;
                _fixedSymbolForPyramid[0][rowIndex]->setVisible(false);
            }
            for (auto rowIndex = _resultIcons.size() - 1; rowIndex >= 0; rowIndex--) {
                auto _icon = _resultIcons[rowIndex];
                _icon->setVisible(false);
                _fixedSymbolForPyramid[0][rowIndex]->setVisible(true);
                _fixedSymbolForPyramid[0][rowIndex]->setAnimation("appear");
                coFunctionWait(150, [this] {});//wait
            }
            coFunctionWait(1016, [this] {});//wait
            for (auto index = 0,size = _fixedSymbolForPyramid[0].size(); index < size; index++) {
                _fixedSymbolForPyramid[0][index]->setAnimation("focus");
                setIconTextForPyramid(_fixedSymbolForPyramid[0][index], 203);
                if (_fixedSymbolForPyramid[0][index]._attach) {
                    _fixedSymbolForPyramid[0][index]._attach->setAnimation("focus");
                }
            }
        });
}

void CSlot346PiggyAdventure::removeFixedSymbolForPyramid()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            if (_fixedSymbolForPyramid[reelIndex][rowIndex]) {
                _fixedSymbolForPyramid[reelIndex][rowIndex]->removeFromParent();
                _fixedSymbolForPyramid[reelIndex][rowIndex] = nullptr;
            }
        }
    }
}

void CSlot346PiggyAdventure::replaceIconLowForPyramid()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto icons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            _slotReelWrapper->replaceResultIconById(rowIndex, CMiscUtils::getRandomIntInclusive(10004, 10011));
        }
    }
}

/**
 * @protected
 * 모든 히트 심볼 포지션 조회
 */
void CSlot346PiggyAdventure::_findAllHitIconsForPyramid()
{
    auto _hitIcons = [];
    if (_currentResult && _currentResult->spinResult) {
        auto length = _currentResult->spinResult->payout->lines ? _currentResult->spinResult->payout->lines.size() : 0;
        for (auto lineIndex = 0; lineIndex < length; lineIndex++) {
            auto _hitline = _currentResult->spinResult->payout->lines[lineIndex];

            for (auto posIndex = 0, posLen = _hitline.position.size(); posIndex < posLen; posIndex++) {
                auto _pos = _hitline.position[posIndex];
                auto _reelIndex = _pos[0];
                auto _rowIndex = _pos[1];

                auto _icon = _fixedSymbolForPyramid[_reelIndex][_rowIndex];
                if (!ArrayUtils::contains(_hitIcons, _icon)) {
                    _hitIcons.push_back(_icon);
                }
            }
        }
    }
    return _hitIcons;
}

void CSlot346PiggyAdventure::isPyramidScatter(_iconId)
{
    return _iconId == 203 || _iconId == 210;
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 아틀란티스 함수
////////////////////////////////////////////////////////////////////////////////////////////
//싱글릴(뱀파이어)
void CSlot346PiggyAdventure::_createSlotReelWrapperForAtlantis(instance, options)
{
    options.onBounceStopSpin = _onBounceStopSpinForAtlantis.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    auto randomStart = CMiscUtils::getRandomIntInclusive(0, options.reelArray.size() - 1);
    options.startPosition = randomStart;
    options.spineContainer = _spineContainer;
    options.findIndexFunc = findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureAtlantisReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
void CSlot346PiggyAdventure::_onBounceStopSpinForAtlantis(_slotReelWrapper)
{
    const reelIndex = _slotReelWrapper->reelIndex;
    // 릴 스핀 종료 재생
    auto _resultIcons = _slotReelWrapper->getResultIcons();
    auto scatterAppear = false;
    auto size = reelIndex == 0 ? _reelWindowHeight : 1;
    for (auto rowIndex = 0; rowIndex < size; rowIndex++) {
        auto _icon = _resultIcons[rowIndex];
        auto _iconId = _icon->_dug_icon_id;
        if (!isAtlantisScatter(_iconId)) {
            continue;
        }
        _icon->setVisible(false);
        auto spineName = "s346_bonus_b_icon_wild";
        auto action = "appear";
        _fixedSymbolForAtlantis[reelIndex][rowIndex] = _spineContainer->borrow(spineName);
        layer_reel_frame_atlantis->addChild(_fixedSymbolForAtlantis[reelIndex][rowIndex]);
        _fixedSymbolForAtlantis[reelIndex][rowIndex]->setAnimation(action);
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
        _fixedSymbolForAtlantis[reelIndex][rowIndex]->adjustPosition(_reel, rowIndex, 1);
        _fixedSymbolForAtlantis[reelIndex][rowIndex]._dug_icon_id = _iconId;
        _fixedSymbolForAtlantis[reelIndex][rowIndex]._reelIndex = reelIndex;
        _fixedSymbolForAtlantis[reelIndex][rowIndex]._rowIndex = rowIndex;
        if (_iconId == 300) {
            scatterAppear = true;
            scatterAppearForAtlantis = true;
            _fixedSymbolForAtlantis[reelIndex][rowIndex].multiple = 0;
            auto spine = _spineContainer->borrow("s346_icon_bonus_b_multiple");
            _fixedSymbolForAtlantis[reelIndex][rowIndex]->addAsyncLinkSpine(spine, "num");
            _fixedSymbolForAtlantis[reelIndex][rowIndex]._attach = spine;
            spine->setVisible(false);
        }
    }
    if (scatterAppear) {
        playEffectSound("snd_multiple_appear");
    }
}

void CSlot346PiggyAdventure::atlantisProc()
{
    getHitPaylinesForAtlantis();
    if (paylineInfoForAtlantis.size() > 0 && scatterAppearForAtlantis) {
        for (auto index = 0,size = paylineInfoForAtlantis.size(); index < size; index++) {
            auto paylineIndex = paylineInfoForAtlantis[index];
            auto payline = paylinesForAtlantis[paylineIndex];
            for (auto i = 0; i < 4; i++) {
                if (i == 0) {
                    playEffectSpine(
                        "s346_icon_bonus_b_hit_box",
                        _fixedSymbolForAtlantis[0][payline[i]],
                        "hit");
                }
                else {
                    playEffectSpine(
                        "s346_icon_bonus_b_hit_box",
                        _fixedSymbolForAtlantis[(payline[i] + 1) + (9 * (i - 1))][0],
                        "hit");
                }
            }
            if (!_payHitAniForAtlantis[paylineIndex]) {
                _payHitAniForAtlantis[paylineIndex] = _spineContainer->borrow("s346_ui_bonus_b_pay_hit");
                _payBoxNumForAtlantis->addAsyncLinkSpine(_payHitAniForAtlantis[paylineIndex], "num_" + (paylineIndex + 1));
                _payHitAniForAtlantis[paylineIndex]->setAnimation("hit");
                _payHitAniForAtlantis[paylineIndex]->addAnimation("loop", true);
            }
        }
        playEffectSound("snd_payline_b1");
        _spinProcessQueue->next(1000);
    }
    else {
        _spinProcessQueue->next(0);
    }
}

void CSlot346PiggyAdventure::atlantisProc1()
{
    if (scatterAppearForAtlantis) {
        coFunctionWait(0, [this]{
                playEffectSound("snd_multiple_hit");
                for (auto reelIndex = 1; reelIndex < atlantisReelWidth; reelIndex++) {
                    for (auto rowIndex = 0,size = _fixedSymbolForAtlantis[reelIndex].size(); rowIndex < size; rowIndex++) {
                        if (_fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                            auto _icon = _fixedSymbolForAtlantis[reelIndex][rowIndex];
                            if (_icon->multiple == 0) {
                                // auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                                _icon->setAnimation("lock");

                                // _icon->parent->addChild(_icon);
                                // _icon->adjustPosition(_reel, rowIndex, 1);
                            }
                            // setIconTextForPyramid(_icon, _icon->_dug_icon_id);
                        }
                    }
                }
            });
        coFunctionWait(1100, [this] {
                if (scatterAppearForAtlantis && _staySpine) {
                    _staySpine->setAnimation("hit");
                    _staySpine->addAnimation("loop", true);
                }
                auto max = 0;
                for (auto reelIndex = 1; reelIndex < atlantisReelWidth; reelIndex++) {
                    for (auto rowIndex = 0,size = _fixedSymbolForAtlantis[reelIndex].size(); rowIndex < size; rowIndex++) {
                        if (_fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                            auto _icon = _fixedSymbolForAtlantis[reelIndex][rowIndex];
                            // auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                            _icon->multiple += 1;
                            if (_icon->multiple > max) {
                                max = _icon->multiple;
                            }
                            auto action = "multiple_1";
                            if (reelIndex > 9 && reelIndex < 19) {
                                action = "multiple_2";
                            }
                            else if (reelIndex > 18) {
                                action = "multiple_3";
                            }
                            if (_icon->multiple > 1) {
                                auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                                _icon->setAnimation(action);
                                _icon->parent->addChild(_icon);
                                _icon->adjustPosition(_reel, rowIndex, 1);
                            }
                            if (_icon->_attach) {
                                _icon->_attach->setAnimation(_icon->multiple + "x");
                                _icon->_attach->setVisible(true);
                            }
                            // _icon->parent->addChild(_icon);
                            // _icon->adjustPosition(_reel, rowIndex, 1);
                        }
                        // setIconTextForPyramid(_icon, _icon->_dug_icon_id);
                    }
                }
                if (max > 6) {
                    max = 6;
                }
                if (max >= 2) {
                    playEffectSound("snd_multiple_pump_b" + (max - 1));
                }
            });
        _spinProcessQueue->next(3000);
    }
    else {
        _spinProcessQueue->next(0);
    }
}

void CSlot346PiggyAdventure::atlantisProc2()
{
    if (paylineInfoForAtlantis.size() > 0 && scatterAppearForAtlantis) {
        playEffectSound("snd_payline_b2");
        for (auto index = 0,size = paylineInfoForAtlantis.size(); index < size; index++) {
            auto paylineIndex = paylineInfoForAtlantis[index];
            auto payline = paylinesForAtlantis[paylineIndex];
            playEffectSpine(
                "s346_ui_pay_line_" + (paylineIndex + 1),
                eff_container,
                "hit");
            auto delay = 166;
            for (auto i = 1; i < 4; i++) {
                coFunctionWait(delay, [this] {
                        _fixedSymbolForAtlantis[(payline[i] + 1) + (9 * (i - 1))][0]->setAnimation("payline");
                    });
                delay += 166;
            }
        }
        _spinProcessQueue->next(566);
    }
    else {
        _spinProcessQueue->next(0);
    }
}

void CSlot346PiggyAdventure::atlantisProc3()
{
    if (paylineInfoForAtlantis.size() > 0 && scatterAppearForAtlantis) {
        for (auto index = 0,size = respinAwardForAtlantis.size(); index < size; index++) {
            auto pay = _avgTotalBet * respinAwardForAtlantis[index] / 10;
            if (pay == 0) {
                continue;
            }
            if (_payBoxTextForAtlantis[index]) {
                auto _duration = 1;
                auto _value = {value : 0, init : linePayForAtlantis[index], result : (pay), options : options};
                TweenLite.to(_value, _duration, {
                    value : 1,
                    onUpdate : onUpdateLinePayout.bind(this),
                    onUpdateParams : [ _value, index ],
                });
                linePayForAtlantis[index] = pay;
            }
            else {
                _payBoxTextForAtlantis[index] = _payBoxNumForAtlantis->addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
                _payBoxTextForAtlantis[index]->setScaleX(0.5);
                _payBoxTextForAtlantis[index]->setScaleY(-1 * 0.5);
                _payBoxTextForAtlantisBig[index] = _payBoxNumForAtlantisBig->addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, eventListener);
                _payBoxTextForAtlantisBig[index]->setScaleX(0.5);
                _payBoxTextForAtlantisBig[index]->setScaleY(-1 * 0.5);
                _payBoxTextForAtlantisBig[index]->setVisible(false);
                // 금액 업데이트
                auto _duration = 1;
                auto _value = {value : 0, init : 0, result : (pay), options : options};
                TweenLite.to(_value, _duration, {
                    value : 1,
                    onUpdate : onUpdateLinePayout.bind(this),
                    onUpdateParams : [ _value, index ],
                });
                linePayForAtlantis[index] = pay;
            }
        }
        _spinProcessQueue->next(2000);
    }
    else {
        _spinProcessQueue->next(0);
    }
}

void CSlot346PiggyAdventure::atlantisProc4()
{
    updateStaySymbolForAtlantis();
    _spinProcessQueue->next(100);
}

void CSlot346PiggyAdventure::atlantisPayoutProc()
{
    if (_payoutSpine) {
        stopBGMSound();
        playEffectSound("snd_hit_pot_b");
        _payoutSpine->setAnimation("in") _payoutSpine->addAnimation("loop", true)
    }
    _spinProcessQueue->next(2000);
}

void CSlot346PiggyAdventure::atlantisPayoutProc1()
{
    removeHitAniForAtlantis();
    if (_payBoxNumForAtlantis) {
        eff_container2->addChild(_payBoxNumForAtlantis);
        _payBoxNumForAtlantis->setAnimation("move");
        eff_container2->addChild(_payBoxNumForAtlantisBig);
        _payBoxNumForAtlantisBig->setAnimation("move");
    }
    if (_staySpine) {
        eff_container3->addChild(_staySpine);
        _payBoxNumForAtlantisBig->setAnimation("loop", true);
    }
    _spinProcessQueue->next(2000);
}

void CSlot346PiggyAdventure::atlantisPayoutProc2()
{
    auto count = payoutCount();
    auto _duration = count * 0.5 + 0.7;
    auto _totalAward = totalAward();
    _payoutTextForAtlantis = _payoutSpine->addBitmapText("num", {size : 74, font : "s346_num_congratulations", align : "center", alignY : "center"}, eventListener);
    _payoutTextForAtlantis->setString("0")
    _payoutTextForAtlantis->setAnchorPoint(ccp(0.5, 0.5));
    setOnlyBonusPotPay(true);
    coFunctionWait(300, [this] {
            auto kind = getPayoutBeforeEffectKind(_totalAward / _avgTotalBet);
            if (kind != "none") {
                m_bottomPanel->playPayoutWinEffect(kind);
                playPayoutBeforeEffectSound(kind);
            }
            stopBGMSound();
            playBGMSound("bgm_payout_b1");
            payoutType = "b1";
            _payoutTween = CMiscUtils::playCoinAnimationABB({
                target : _payoutTextForAtlantis,
                duration : _duration,
                align : "center",
                start : 0,
                end : _totalAward,
                prefix : "_"
            });
            playPayoutIncrement({
                delay : 2000,
                duration : _duration,
                init : 0,
                result : _totalAward,
                payoutType : "update_bonuspot"
            });
        });

    coFunctionWait(0, [this]{
            for (auto index = 0,size = respinAwardForAtlantis.size(); index < size; index++) {
                if (_payBoxTextForAtlantis[index]) {
                    auto position = getSlotLocalPosision(_payBoxNumForAtlantis, "num_" + (index + 1));
                    playEffectSound("snd_pot_collect");
                    TweenMax.to(_payBoxTextForAtlantis[index], 0.3,
                                {
                                    ease : Power0.easeOut,
                                    x : -position[0],
                                    y : -position[1]
                                });
                    TweenMax.to(_payBoxTextForAtlantisBig[index], 0.3,
                                {
                                    ease : Power0.easeOut,
                                    x : -position[0],
                                    y : -position[1]
                                });
                    coFunctionWait(300, [this] {
                            _payBoxTextForAtlantis[index]->setVisible(false);
                            _payBoxTextForAtlantisBig[index]->setVisible(false);
                            _payoutSpine->setAnimation("hit");
                        });
                    coFunctionWait(500, [this] {});//wait
                }
            }
        });
}

void CSlot346PiggyAdventure::atlantisPayoutProc3()
{
    removePayBoxForAtlantis();
    _payoutTween.kill();
    // _payoutSpine->setAnimation("loop", true);
    _spinProcessQueue->next(0);
}

void CSlot346PiggyAdventure::payoutCount()
{
    auto count = 0;
    for (auto index = 0,size = respinAwardForAtlantis.size(); index < size; index++) {
        if (respinAwardForAtlantis[index] > 0) {
            count++;
        }
    }
    return count;
}

void CSlot346PiggyAdventure::onUpdateLinePayout(value, index)
{
    auto _totalAward = value.init + value.value * (value.result - value.init);
    if (_totalAward >= _avgTotalBet * 10) {
        _payBoxTextForAtlantis[index]->setVisible(false);
        _payBoxTextForAtlantisBig[index]->setVisible(true);
    }
    _payBoxTextForAtlantis[index]->setString(CMiscUtils::abbreviatesNumberOption({num : _totalAward, fractionDigits : 1}))
    _payBoxTextForAtlantis[index]->setAnchorPoint(ccp(0.5, 0.5));
    _payBoxTextForAtlantisBig[index]->setString(CMiscUtils::abbreviatesNumberOption({num : _totalAward, fractionDigits : 1}))
    _payBoxTextForAtlantisBig[index]->setAnchorPoint(ccp(0.5, 0.5));

    _payBoxTextForAtlantis[index]->getPositionY() -= 5;
    _payBoxTextForAtlantisBig[index]->getPositionY() -= 5;
}

void CSlot346PiggyAdventure::updateStaySymbolForAtlantis()
{
    for (auto reelIndex = 0; reelIndex < atlantisReelWidth; reelIndex++) {
        for (auto rowIndex = 0,size = _fixedSymbolForAtlantis[reelIndex].size(); rowIndex < size; rowIndex++) {
            if (_fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                auto _icon = _fixedSymbolForAtlantis[reelIndex][rowIndex];
                _icon->setAnimation("loop", true);
                _icon->parent->addChild(_icon);
                _icon->adjustPosition(_reel, rowIndex, 1);
                // setIconTextForPyramid(_icon, _icon->_dug_icon_id);
            }
        }
    }
}
/*
 * 아틀란티스 아이콘 텍스트
 */
void CSlot346PiggyAdventure::setIconTextForAtlantis(_icon, _iconId, random = false, on = false)
{
    if (_icon == undefined || _iconId < 3000) {
        return;
    }
    auto fixPay = 0;
    _iconId = _iconId % 3000;
    auto pay = [ 5, 6, 7, 8, 9, 10, 20, 30, 40, 50 ];
    fixPay = _avgTotalBet * pay[_iconId] / 10;
    auto _text = _icon->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
    _text->setString(CMiscUtils::abbreviatesNumberOption({num : fixPay, fractionDigits : 1}))
    _text->setAnchorPoint(ccp(0.5, 0.5));
    _icon->addAsyncLinkSpine(_text);
}

void CSlot346PiggyAdventure::getHitPaylinesForAtlantis()
{
    paylineInfoForAtlantis = [];
    for (auto index = 0,size = paylinesForAtlantis.size(); index < size; index++) {
        auto count = 0;
        for (auto reelIndex = 0,size = paylinesForAtlantis[index].size(); reelIndex < size; reelIndex++) {
            if (fixedPlaylineForSuper[reelIndex][paylinesForAtlantis[index][reelIndex]] > 0) {
                count++;
            }
            else {
                break;
            }
        }
        if (count > 3) {
            paylineInfoForAtlantis.push_back(index);
        }
    }
}

void CSlot346PiggyAdventure::removeFixedSymbolForAtlantis()
{
    for (auto reelIndex = 0; reelIndex < atlantisReelWidth; reelIndex++) {
        for (auto rowIndex = 0,size = _fixedSymbolForAtlantis[reelIndex].size(); rowIndex < size; rowIndex++) {
            if (_fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                _fixedSymbolForAtlantis[reelIndex][rowIndex]->removeFromParent();
                _fixedSymbolForAtlantis[reelIndex][rowIndex] = nullptr;
            }
        }
    }
}

void CSlot346PiggyAdventure::removePayBoxForAtlantis()
{
    for (auto index = 0,size = _payBoxTextForAtlantis.size(); index < size; index++) {
        if (_payBoxTextForAtlantis[index]) {
            _payBoxTextForAtlantis[index]->removeFromParent();
            _payBoxTextForAtlantis[index] = nullptr;
        }
    }
    for (auto index = 0,size = _payBoxTextForAtlantisBig.size(); index < size; index++) {
        if (_payBoxTextForAtlantisBig[index]) {
            _payBoxTextForAtlantisBig[index]->removeFromParent();
            _payBoxTextForAtlantisBig[index] = nullptr;
        }
    }
    if (_payBoxNumForAtlantis) {
        _payBoxNumForAtlantis->removeFromParent();
        _payBoxNumForAtlantis = nullptr;
    }
    if (_payBoxNumForAtlantisBig) {
        _payBoxNumForAtlantisBig->removeFromParent();
        _payBoxNumForAtlantisBig = nullptr;
    }
    removeHitAniForAtlantis();
}

void CSlot346PiggyAdventure::removeHitAniForAtlantis()
{
    for (auto index = 0,size = _payHitAniForAtlantis.size(); index < size; index++) {
        if (_payHitAniForAtlantis[index]) {
            _payHitAniForAtlantis[index]->removeFromParent();
            _payHitAniForAtlantis[index] = nullptr;
        }
    }
}

void CSlot346PiggyAdventure::removePayoutSpine()
{
    if (_payoutSpine) {
        _payoutSpine->removeFromParent();
        _payoutSpine = nullptr;
    }
}

void CSlot346PiggyAdventure::superFreespinStartAnimationForAtlantis()
{
    getFeaturedInfo();

    _staySpine->setAnimation("in");
    _staySpine->addAnimation("side_in");
    _staySpine->addAnimation("loop", true);

    coFunctionWait(0, [this]{
            playEffectSound("snd_1st_reel_b");
            coFunctionWait(833, [this] {});//wait
            auto _slotReelWrapper = allReels[spinType][0];
            auto _resultIcons = _slotReelWrapper->getResultIcons();
            for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
                auto spineName = "s346_icon_bonus_b_payout";
                _fixedSymbolForAtlantis[0][rowIndex] = _spineContainer->borrow(spineName);
                layer_reel_frame_atlantis->addChild(_fixedSymbolForAtlantis[0][rowIndex]);
                auto _reel = _getTypeReel(0, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                _fixedSymbolForAtlantis[0][rowIndex]->adjustPosition(_reel, rowIndex, 1);
                _fixedSymbolForAtlantis[0][rowIndex]._dug_icon_id = fixedPlaylineForSuper[0][rowIndex];
                _fixedSymbolForAtlantis[0][rowIndex]._reelIndex = 0;
                _fixedSymbolForAtlantis[0][rowIndex]._rowIndex = rowIndex;
                _fixedSymbolForAtlantis[0][rowIndex]->setAnimation("appear");
                auto _icon = _resultIcons[rowIndex];
                _icon->setVisible(false);
                setIconTextForAtlantis(_fixedSymbolForAtlantis[0][rowIndex], fixedPlaylineForSuper[0][rowIndex]);
                coFunctionWait(90, [this] {});//wait
            }
        });
}

void CSlot346PiggyAdventure::recoveryProcForAtlantis()
{
    getFeaturedInfo();

    _payBoxNumForAtlantis = _spineContainer->borrow("s346_ui_bonus_b_pay");
    eff_container3->addChild(_payBoxNumForAtlantis);
    _payBoxNumForAtlantisBig = _spineContainer->borrow("s346_ui_bonus_b_pay");
    eff_container3->addChild(_payBoxNumForAtlantisBig);
    _payBoxTextForAtlantis = create1DArray(paylinesForAtlantis.size(), null);
    _payBoxTextForAtlantisBig = create1DArray(paylinesForAtlantis.size(), null);
    _payHitAniForAtlantis = create1DArray(paylinesForAtlantis.size(), null);
    linePayForAtlantis = create1DArray(paylinesForAtlantis.size(), 0);

    _staySpine = _spineContainer->borrow("s346_ui_bonus_b_pig");
    _staySpine->setAnimation("loop", true);
    eff_container2->addChild(_staySpine);
    playPayoutSpineForAtlantis();
    replaceIconLowForAtlantis();
    for (auto reelIndex = 0; reelIndex < atlantisReelWidth; reelIndex++) {
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
        for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            auto _changeReelIndex = reelIndex == 0 ? 0 : (reelIndex - 1) / 9 + 1;
            auto _changerowIndex = reelIndex == 0 ? rowIndex : (reelIndex - 1) % 9 auto _iconId = fixedPlaylineForSuper[_changeReelIndex][_changerowIndex];
            // 1->1~9
            // 2->10~18
            // 3->19~27
            if (!isAtlantisScatter(_iconId)) {
                continue;
            }
            _icon->setVisible(false);
            auto spineName = "s346_bonus_b_icon_wild";
            if (_iconId >= 3000) {
                spineName = "s346_icon_bonus_b_payout";
            }
            _fixedSymbolForAtlantis[reelIndex][rowIndex] = _spineContainer->borrow(spineName);
            layer_reel_frame_atlantis->addChild(_fixedSymbolForAtlantis[reelIndex][rowIndex]);
            _fixedSymbolForAtlantis[reelIndex][rowIndex]->adjustPosition(_reel, rowIndex, 1);
            _fixedSymbolForAtlantis[reelIndex][rowIndex]._dug_icon_id = _iconId;
            _fixedSymbolForAtlantis[reelIndex][rowIndex]._reelIndex = reelIndex;
            _fixedSymbolForAtlantis[reelIndex][rowIndex]._rowIndex = rowIndex;
            if (_iconId >= 3000) {
                setIconTextForAtlantis(_fixedSymbolForAtlantis[0][rowIndex], fixedPlaylineForSuper[_changeReelIndex][_changerowIndex]);
            }
            else {
                _fixedSymbolForAtlantis[reelIndex][rowIndex].multiple = _iconId % 300 == 0 ? 1 : _iconId % 300;
                auto spine = _spineContainer->borrow("s346_icon_bonus_b_multiple");
                _fixedSymbolForAtlantis[reelIndex][rowIndex]->addAsyncLinkSpine(spine, "num");
                _fixedSymbolForAtlantis[reelIndex][rowIndex]._attach = spine;
                if (_fixedSymbolForAtlantis[reelIndex][rowIndex]._attach) {
                    _fixedSymbolForAtlantis[reelIndex][rowIndex]._attach->setAnimation(_fixedSymbolForAtlantis[reelIndex][rowIndex].multiple + "x");
                    _fixedSymbolForAtlantis[reelIndex][rowIndex]._attach->setVisible(true);
                }
            }
        }
    }
    for (auto index = 0,size = respinAwardForAtlantis.size(); index < size; index++) {
        auto pay = _avgTotalBet * respinAwardForAtlantis[index] / 10;
        if (pay == 0) {
            continue;
        }
        _payHitAniForAtlantis[index] = _spineContainer->borrow("s346_ui_bonus_b_pay_hit");
        _payBoxNumForAtlantis->addAsyncLinkSpine(_payHitAniForAtlantis[index], "num_" + (index + 1));
        _payHitAniForAtlantis[index]->setAnimation("loop", true);
        _payBoxTextForAtlantis[index] = _payBoxNumForAtlantis->addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
        _payBoxTextForAtlantis[index]->setScaleX(0.5);
        _payBoxTextForAtlantis[index]->setScaleY(-1 * 0.5);
        _payBoxTextForAtlantisBig[index] = _payBoxNumForAtlantisBig->addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, eventListener);
        _payBoxTextForAtlantisBig[index]->setScaleX(0.5);
        _payBoxTextForAtlantisBig[index]->setScaleY(-1 * 0.5);
        _payBoxTextForAtlantisBig[index]->setVisible(false);
        if (pay >= _avgTotalBet * 10) {
            _payBoxTextForAtlantis[index]->setVisible(false);
            _payBoxTextForAtlantisBig[index]->setVisible(true);
        }
        _payBoxTextForAtlantis[index]->setString(CMiscUtils::abbreviatesNumberOption({num : pay, fractionDigits : 1}))
        _payBoxTextForAtlantis[index]->setAnchorPoint(ccp(0.5, 0.5));
        _payBoxTextForAtlantisBig[index]->setString(CMiscUtils::abbreviatesNumberOption({num : pay, fractionDigits : 1}))
        _payBoxTextForAtlantisBig[index]->setAnchorPoint(ccp(0.5, 0.5));
        _payBoxTextForAtlantis[index]->getPositionY() -= 5;
        _payBoxTextForAtlantisBig[index]->getPositionY() -= 5;
        linePayForAtlantis[index] = pay;
    }
    updateStaySymbolForAtlantis();
}

void CSlot346PiggyAdventure::replaceIconLowForAtlantis()
{
    for (auto reelIndex = 0; reelIndex < atlantisReelWidth; reelIndex++) {
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
            _slotReelWrapper->replaceResultIconById(rowIndex, CMiscUtils::getRandomIntInclusive(10004, 10011));
        }
    }
}

void CSlot346PiggyAdventure::isAtlantisScatter(iconId)
{
    return (iconId >= 300 && iconId <= 400) || (iconId >= 3000 && iconId <= 3100);
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 오리엔탈 함수
////////////////////////////////////////////////////////////////////////////////////////////
//일반릴(카멜레온)
void CSlot346PiggyAdventure::_createSlotReelWrapperForOriental(instance, options)
{
    options.onBounceStopSpin = _onBounceStopSpinForOriental.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    auto randomStart = CMiscUtils::getRandomIntInclusive(0, options.reelArray.size() - 1);
    options.startPosition = randomStart;
    options.spineContainer = _spineContainer;
    options.findIndexFunc = findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureOrientalReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
void CSlot346PiggyAdventure::_onBounceStopSpinForOriental(_slotReelWrapper)
{
    const reelIndex = _slotReelWrapper->reelIndex;
    // 릴 스핀 종료 재생
    auto _resultIcons = _slotReelWrapper->getResultIcons();
    auto scatterAppear = false;
    for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
        auto _icon = _resultIcons[rowIndex];
        auto _iconId = _icon->_dug_icon_id;
        if (!isOrientalScatter(_iconId)) {
            continue;
        }
        if (_iconId == 420) {
            _icon->setVisible(false);
        }
        else {
            continue;
        }
        scatterAppear = true;
        auto spineName = "s346_icon_bonus_c_scatter";
        auto action = "appear";
        _fixedSymbolForOriental[reelIndex][rowIndex] = _spineContainer->borrow(spineName);
        layer_reel_frame_oriental->addChild(_fixedSymbolForOriental[reelIndex][rowIndex]);
        _fixedSymbolForOriental[reelIndex][rowIndex]->setAnimation(action);
        auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);
        _fixedSymbolForOriental[reelIndex][rowIndex]->adjustPosition(_reel, rowIndex, 1);
        _fixedSymbolForOriental[reelIndex][rowIndex]._dug_icon_id = _iconId;
        _fixedSymbolForOriental[reelIndex][rowIndex]._reelIndex = reelIndex;
        _fixedSymbolForOriental[reelIndex][rowIndex]._rowIndex = rowIndex;
    }
    playEffectSound("snd_reel_stop_c");
    if (scatterAppear) {
        playEffectSound("snd_pig_appear");
    }
}

void CSlot346PiggyAdventure::orientalPayoutProc()
{
    setOnlyBonusPotPay(true);
    bonuspot_number = orientalTotalAward;
    orientalCoinCount = 0;
    orientalMultiCount = 0;
    fixedPayForOriental = create2DArray(_reelWidth, _reelWindowHeight, 0);
    _spinProcessQueue->next(500);
}

void CSlot346PiggyAdventure::orientalPayoutProc1()
{
    updateStaySymbolForOriental();
    if (orientalCoinCount > 0 || orientalMultiCount > 0) {
        auto delay = 1166 + (orientalCoinCount + orientalMultiCount * 200) + 1333 + 500;
        for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
            if (!ArrayUtils::contains(hitReelForOriental, reelIndex)) {
                continue;
            }
            auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);
            auto _slotReelWrapper = allReels[spinType][reelIndex];
            auto _resultIcons = _slotReelWrapper->getResultIcons();
            for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                auto _icon = _resultIcons[rowIndex];
                auto iconId = _icon->_dug_icon_id;
                if (iconId == 420 && _fixedSymbolForOriental[reelIndex][rowIndex]) {
                    focusAniForOriental(reelIndex, rowIndex);
                }
            }
        }
        focusSoundForOriental();
        _spinProcessQueue->next(delay);
    }
    else {
        _spinProcessQueue->next(0);
    }
}

void CSlot346PiggyAdventure::orientalPayoutProc2()
{
    if (orientalCoinCount > 0) {
        auto delay = (orientalCoinCount * 533) + 833 + 500;
        for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
            if (!ArrayUtils::contains(hitReelForOriental, reelIndex)) {
                continue;
            }
            auto _slotReelWrapper = allReels[spinType][reelIndex];
            auto _resultIcons = _slotReelWrapper->getResultIcons();
            for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                auto _icon = _resultIcons[rowIndex];
                auto iconId = _icon->_dug_icon_id;
                if (iconId == 420 && _fixedSymbolForOriental[reelIndex][rowIndex]) {
                    coinPayoutForOriental(reelIndex, rowIndex);
                }
            }
        }
        coinPayoutSoundForOriental();
        _spinProcessQueue->next(delay);
    }
    else {
        _spinProcessQueue->next(0);
    }
}

void CSlot346PiggyAdventure::orientalPayoutProc3()
{
    if (orientalMultiCount > 0) {
        coFunctionWait(0, [this]{
                for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
                    if (!ArrayUtils::contains(hitReelForOriental, reelIndex)) {
                        continue;
                    }
                    auto _slotReelWrapper = allReels[spinType][reelIndex];
                    auto _resultIcons = _slotReelWrapper->getResultIcons();
                    for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                        auto _icon = _resultIcons[rowIndex];
                        auto iconId = _icon->_dug_icon_id;
                        if (iconId == 420 && _fixedSymbolForOriental[reelIndex][rowIndex]) {
                            _fixedSymbolForOriental[reelIndex][rowIndex].parent->addChild(_fixedSymbolForOriental[reelIndex][rowIndex]);
                            auto count = 1;
                            for (auto index = rowIndex - 1; index >= 0; index--) {
                                auto _aIcon = _resultIcons[index];
                                auto iconId = _aIcon->_dug_icon_id;
                                if (iconId == 440 && _fixedSymbolForOriental[reelIndex][index]) {
                                    _aIcon->setVisible(true);
                                    _aIcon->setAnimation("dim");
                                    _fixedSymbolForOriental[reelIndex][index]->setVisible(true);
                                    layer_reel_up->addChild(_fixedSymbolForOriental[reelIndex][index]);
                                    _fixedSymbolForOriental[reelIndex][index]->setAnimation("save_" + count);
                                    auto type = getMultipotForSuper(fixedPayoutForSuper[reelIndex][index]);
                                    playEffectSound("snd_" + multipots[type]);
                                    if (_fixedSymbolForOriental[reelIndex][index]._attach) {
                                        _fixedSymbolForOriental[reelIndex][index]._attach->setAnimation("save");
                                        _fixedSymbolForOriental[reelIndex][index]._attach->setVisible(true);
                                    }
                                    scatterSaveForOriental(reelIndex, rowIndex, index, true);
                                    playEffectSound("snd_pig_text");
                                    coFunctionWait(3666, [this] {});//wait
                                }
                                count++;
                            }
                        }
                    }
                }
                _spinProcessQueue->next(500);
            });
    }
    else {
        _spinProcessQueue->next(0);
    }
}

void CSlot346PiggyAdventure::orientalPayoutProc4()
{
    if (m_bottomPanel) {
        m_bottomPanel->playPayoutEffect();
    }
    orientalTotalAwardPre = orientalTotalAward;
    playEffectSound("snd_pig_pay");
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        if (!ArrayUtils::contains(hitReelForOriental, reelIndex)) {
            continue;
        }
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            auto iconId = _icon->_dug_icon_id;
            if (iconId == 420 && _fixedSymbolForOriental[reelIndex][rowIndex]) {
                _fixedSymbolForOriental[reelIndex][rowIndex]->setAnimation("hit_in");
                _fixedSymbolForOriental[reelIndex][rowIndex]->addAnimation("hit_loop", true);
            }
        }
    }
    if (_staySpine) {
        _staySpine->setAnimation("hit", true);
    }
    _spinProcessQueue->next(3000);
}

void CSlot346PiggyAdventure::orientalPayoutProc5()
{
    if (_staySpine) {
        _staySpine->setAnimation("side_loop", true);
    }
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        if (!ArrayUtils::contains(hitReelForOriental, reelIndex)) {
            continue;
        }
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            auto iconId = _icon->_dug_icon_id;
            if (iconId == 420 && _fixedSymbolForOriental[reelIndex][rowIndex]) {
                _icon->setVisible(true);
                _icon->setAnimation("on2");
                auto fixPay = fixedPayForOriental[reelIndex][rowIndex];
                auto _text = nullptr;
                auto slot = _icon->getSlotContainer("num");
                if (slot && slot._text) {
                    slot._dug_init_textfield = false;
                    slot._text->removeFromParent();
                    delete slot._text;
                    slot._text = nullptr;
                }
                if (fixPay >= _avgTotalBet * 5) {
                    _text = _icon->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, eventListener);
                }
                else {
                    _text = _icon->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
                }
                _text->setScaleX(0.625);
                _text->setScaleY(-1 * 0.625);
                _text->setString(CMiscUtils::MakeUnitString(fixPay))
                _text->setAnchorPoint(ccp(0.5, 0.5));
                _icon->_attach = _text;
            }
        }
    }
    removeFixedSymbolForOriental();
    _spinProcessQueue->next(10);
}

void CSlot346PiggyAdventure::focusAniForOriental(reelIndex, rowIndex)
{
    coFunctionWait(0, [this]{
            auto _slotReelWrapper = allReels[spinType][reelIndex];
            auto _resultIcons = _slotReelWrapper->getResultIcons();
            auto _reel = _getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);

            if (_fixedSymbolForOriental[reelIndex][rowIndex]) {
                _fixedSymbolForOriental[reelIndex][rowIndex]->setAnimation("focus");
                _fixedSymbolForOriental[reelIndex][rowIndex]->addAnimation("stay2", true);
                auto _text = _fixedSymbolForOriental[reelIndex][rowIndex]->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
                _text->setString(CMiscUtils::MakeUnitString(0))
                _text->setAnchorPoint(ccp(0.5, 0.5));
                fixedPayForOriental[reelIndex][rowIndex] = 0;
                if (rowIndex > 0) {
                    auto _spine = _spineContainer->borrow("s346_ui_bonus_c_reel_eff2");
                    _spine->setAnimation("focus_" + rowIndex);
                    layer_reel_frame_oriental->addChild(_spine);
                    _spine->adjustPosition(_reel, rowIndex, 1);
                    _spine->addAnimationComplete("focus_" + rowIndex,[this](string entry) {
                            coFunctionWait(0, [this] {
                                    if (_spine) {
                                        _spine->removeFromParent();
                                    }
                                });
                        });
                }
                coFunctionWait(1166, [this] {});//wait
            }
            for (auto index = rowIndex - 1; index >= 0; index--) {
                auto _icon = _resultIcons[index];
                auto iconId = _icon->_dug_icon_id;
                if (iconId == 430 || iconId == 440) {
                    if (_fixedSymbolForOriental[reelIndex][index]) {
                        _fixedSymbolForOriental[reelIndex][index]->removeFromParent();
                        _fixedSymbolForOriental[reelIndex][index] = nullptr;
                    }
                    auto spineName = "s346_icon_bonus_c_coin";
                    if (iconId == 440) {
                        spineName = "s346_icon_bonus_c_gem";
                    }
                    _fixedSymbolForOriental[reelIndex][index] = _spineContainer->borrow(spineName);
                    layer_reel_frame_oriental.animation->addChild(_fixedSymbolForOriental[reelIndex][index]);
                    _fixedSymbolForOriental[reelIndex][index]->adjustPosition(_reel, index, 1);
                    _fixedSymbolForOriental[reelIndex][index]._dug_icon_id = iconId;
                    _fixedSymbolForOriental[reelIndex][index]._reelIndex = reelIndex;
                    _fixedSymbolForOriental[reelIndex][index]._rowIndex = index;
                    setIconTextForOriental(_fixedSymbolForOriental[reelIndex][index], iconId);
                    if (_fixedSymbolForOriental[reelIndex][index]) {
                        _icon->setVisible(false);
                        _fixedSymbolForOriental[reelIndex][index]->setAnimation("focus");
                        _fixedSymbolForOriental[reelIndex][index]->addAnimation("loop", true);
                        coFunctionWait(200, [this] {});//wait
                    }
                }
            }
        });
}

void CSlot346PiggyAdventure::focusSoundForOriental()
{
    coFunctionWait(0, [this]{
            playEffectSound("snd_pig_hit");
            coFunctionWait(1166, [this] {});//wait
            playEffectSoundWithOption("snd_line_start", "snd_line_start");
            coFunctionWait(200 * (orientalCoinCount + orientalMultiCount), [this] {});//wait
            stopEffectSound("snd_line_start");
            playEffectSound("snd_coin_shaking");
            playEffectSound("snd_line_end");
        });
}

void CSlot346PiggyAdventure::coinPayoutForOriental(reelIndex, rowIndex)
{
    coFunctionWait(0, [this]{
            auto count = 1;
            auto _slotReelWrapper = allReels[spinType][reelIndex];
            auto _resultIcons = _slotReelWrapper->getResultIcons();
            for (auto index = rowIndex - 1; index >= 0; index--) {
                auto _icon = _resultIcons[index];
                auto iconId = _icon->_dug_icon_id;
                if (iconId == 430 && _fixedSymbolForOriental[reelIndex][index]) {
                    _icon->setVisible(true);
                    _icon->setAnimation("dim");
                    layer_reel_up->addChild(_fixedSymbolForOriental[reelIndex][index]);
                    _fixedSymbolForOriental[reelIndex][index]->setAnimation("save_" + count);
                    coFunctionWait(833, [this] {
                            _fixedSymbolForOriental[reelIndex][index]->setVisible(false);
                        });
                    coFunctionWait(533, [this] {});//wait
                    scatterSaveForOriental(reelIndex, rowIndex, index, false);
                }
                count++;
            }
        });
}

void CSlot346PiggyAdventure::coinPayoutSoundForOriental()
{
    coFunctionWait(0, [this]{
            for (auto _count = 0; _count < orientalCoinCount; _count++) {
                playEffectSound("snd_coin_hit_c" + (_count + 1));
                coFunctionWait(533, [this] {});//wait
            }
        });
}

void CSlot346PiggyAdventure::scatterSaveForOriental(reelIndex, rowIndex, index, multi)
{
    if (_fixedSymbolForOriental[reelIndex][rowIndex]) {
        auto action = "save";
        auto delay = 300;
        if (multi) {
            action = "save_m";
            delay = 2933;
        }
        _fixedSymbolForOriental[reelIndex][rowIndex]->setAnimation(action);
        _fixedSymbolForOriental[reelIndex][rowIndex]->addAnimationComplete(action,[this](string entry) {
                coFunctionWait(0, [this] {
                        _fixedSymbolForOriental[reelIndex][rowIndex]->setAnimation("stay2", true);
                    });
            });
        coFunctionWait(delay, [this] {
                if (m_bottomPanel) {
                    m_bottomPanel->playPayoutEffect();
                }
                if (_fixedSymbolForOriental[reelIndex][rowIndex]._attach) {
                    auto slot = _fixedSymbolForOriental[reelIndex][rowIndex].getSlotContainer("num");
                    if (slot && slot._text) {
                        slot._dug_init_textfield = false;
                        slot._text->removeFromParent();
                        delete slot._text;
                        slot._text = nullptr;
                        _fixedSymbolForOriental[reelIndex][rowIndex]._attach = nullptr;
                    }
                }
                auto fixPay = _avgTotalBet * fixedPayoutForSuper[reelIndex][index] / 10;
                orientalTotalAward += fixPay;
                bonuspot_number = orientalTotalAward;
                fixedPayForOriental[reelIndex][rowIndex] += fixPay;
                auto _text = nullptr;
                if (fixedPayForOriental[reelIndex][rowIndex] >= _avgTotalBet * 5) {
                    _text = _fixedSymbolForOriental[reelIndex][rowIndex]->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, eventListener);
                }
                else {
                    _text = _fixedSymbolForOriental[reelIndex][rowIndex]->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
                }
                _text->setString(CMiscUtils::MakeUnitString(fixedPayForOriental[reelIndex][rowIndex]))
                _text->setAnchorPoint(ccp(0.5, 0.5));
                _fixedSymbolForOriental[reelIndex][rowIndex]._attach = _text;
            });
    }
}

void CSlot346PiggyAdventure::setTintForOriental(_icon, tint)
{
    auto _tint = 0x404C42;
    auto alpha = 0.75;
    if (!tint) {
        _tint = 0xFFFFFF;
        alpha = 1;
    }
    _icon->tint = _tint;
    _icon->setOpacity(255 * alpha);
    if (_icon->_attach) {
        _icon->_attach.tint = _tint;
        _icon->_attach->setOpacity(255 * alpha);
    }
    auto slot = _icon->getSlotContainer("num");
    if (slot && slot._text) {
        slot._text.tint = _tint;
        slot._text->setOpacity(255 * 1);
    }
}

void CSlot346PiggyAdventure::updateStaySymbolForOriental()
{
    auto maxCountCoin = 0;
    auto maxCountMulti = 0;
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _slotReelWrapper = allReels[spinType][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        auto reel_inner = _slotReelWrapper->_reel_inner.children;
        auto coinCount = 0;
        auto multiCount = 0;
        for (auto index = 0,size = reel_inner.size(); index < size; index++) {
            auto _icon = reel_inner[index];
            if (!ArrayUtils::contains(_resultIcons, _icon)) {
                setTintForOriental(_icon, true);
            }
        }
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            auto iconId = _icon->_dug_icon_id;
            if (iconId == 430) {
                coinCount++;
            }
            else if (iconId == 440) {
                multiCount++;
            }
            if (iconId == 420 && _fixedSymbolForOriental[reelIndex][rowIndex]) {
                _fixedSymbolForOriental[reelIndex][rowIndex]->setAnimation("saty", true);
                if (maxCountCoin < coinCount) {
                    maxCountCoin = coinCount;
                }
                if (maxCountMulti < multiCount) {
                    maxCountMulti = multiCount;
                }
                for (auto index = rowIndex + 1; index < _reelWindowHeight; index++) {
                    setTintForOriental(_resultIcons[index], true);
                }
            }
            if (multiCount + coinCount == 9) {
                for (auto index = 0; index < _reelWindowHeight; index++) {
                    setTintForOriental(_resultIcons[index], true);
                }
            }
        }
    }
    orientalCoinCount = maxCountCoin;
    orientalMultiCount = maxCountMulti;
}

void CSlot346PiggyAdventure::recoveryProcForOriental()
{
    getFeaturedInfo();
    orientalTotalAward = superTotalAward;
    orientalTotalAwardPre = orientalTotalAward;
    _staySpine = _spineContainer->borrow("s346_ui_bonus_c_pig");
    _staySpine->setAnimation("side_loop", true);
    eff_container2->addChild(_staySpine);
    replaceIconLowForOriental();
}

/*
 * 오리엔탈 아이콘 텍스트
 */
void CSlot346PiggyAdventure::setIconTextForOriental(_icon, _iconId, random = false, on = false)
{
    if (_icon == undefined || (_iconId != 430 && _iconId != 440)) {
        return;
    }
    _icon->removeLinkSpine();
    if (_icon->_attach) {
        _icon->_attach->removeFromParent();
        _icon->_attach = nullptr;
    }
    auto isMultipot = false;
    auto reelIndex = _icon->_reelIndex;
    auto rowIndex = _icon->_rowIndex;
    auto fixPay = 0;
    auto type = 0;
    if (random) {
        if (_iconId == 430) {
            auto pay = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40 ];
            auto _pay = pay[CMiscUtils::getRandomIntInclusive(0, pay.size() - 1)];
            fixPay = _avgTotalBet * _pay / 10;
        }
        else {
            type = CMiscUtils::getRandomIntInclusive(1, multipots.size() - 1);
            isMultipot = true;
        }
    }
    else {
        fixPay = _avgTotalBet * fixedPayoutForSuper[reelIndex][rowIndex] / 10;
        if (fixedPayoutForSuper[reelIndex][rowIndex] > 40) {
            isMultipot = true;
        }
        type = getMultipotForSuper(fixedPayoutForSuper[reelIndex][rowIndex]);
    }
    if (!isMultipot) {
        auto _text = _icon->addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, eventListener);
        _text->setString(CMiscUtils::MakeUnitString(fixPay))
        _text->setScaleX(0.625);
        _text->setScaleY(-1 * 0.625);
        _text->setAnchorPoint(ccp(0.5, 0.5));
    }
    else {
        auto spineNameMulti = "s346_icon_bonus_c_" + multipots[type];
        if (on) {
            spineNameMulti += "_on";
        }
        auto _multipot = _spineContainer->borrow(spineNameMulti);
        _multipot->setAnimation("on");
        _icon->addLinkSpine(_multipot, "num");
        _icon->_attach = _multipot;
    }
}

void CSlot346PiggyAdventure::removeFixedSymbolForOriental()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            if (_fixedSymbolForOriental[reelIndex][rowIndex]) {
                _fixedSymbolForOriental[reelIndex][rowIndex]->removeFromParent();
                _fixedSymbolForOriental[reelIndex][rowIndex] = nullptr;
            }
        }
    }
}

void CSlot346PiggyAdventure::_setResultIconsAlphaForOriental(alpha)
{
    for (auto reelIndex = 0,size = slotReels.size(); reelIndex < size; reelIndex++) {
        auto _slotReelWrapper = allReels[CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
            auto _icon = _resultIcons[rowIndex];
            _icon->setOpacity(255 * alpha);
        }
    }
}

void CSlot346PiggyAdventure::replaceIconLowForOriental()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _slotReelWrapper = allReels[CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL][reelIndex];
        auto _resultIcons = _slotReelWrapper->getResultIcons();
        for (auto rowIndex = 0,size = _resultIcons.size(); rowIndex < size; rowIndex++) {
            _slotReelWrapper->replaceResultIconById(rowIndex, 430);
            _resultIcons[rowIndex]->setAnimation("on");
        }
    }
}

void CSlot346PiggyAdventure::isOrientalScatter(_iconId)
{
    return _iconId == 420 || _iconId == 430 || _iconId == 440;
}
////////////////////////////////////////////////////////////////////////////////////////////
//기본 디렉션 함수
////////////////////////////////////////////////////////////////////////////////////////////
/**
 * 디렉션 세팅 기본
 */
void CSlot346PiggyAdventure::_setDirections()
{
    getFeaturedInfo();
    auto spinResult = _currentResult->spinResult;
    auto payout = spinResult->payout;
    auto isRealJackpot = isRealJackpot;
    auto bigwin = (payout->isBigwin&& isRealJackpot == false);
    auto totalAward = totalAward();
    if (isPick && !isSuperFreespin) {
        _setDirection("_step_Pick_Proc");
    }
    if (isSuperFreespin) {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            _setDirection("_step_Pyramid_Proc");
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            _setDirection("_step_Atlantis_Proc");
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            _setDirection("_step_Oriental_Payout");
        }
    }
    if (spinResult->freespin) {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            _setDirection("_step_Bonus_Proc");
        }
    }
    if (totalAward > 0) {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            _setDirection("_step_Payout");
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            _setDirection("_step_Bonus_Payout");
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            _setDirection("_step_Pyramid_Payout");
        }
        else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            _setDirection("_step_Atlantis_Payout");
        }
    }
    auto isJackpot = (payout->isJackpot || jackpotObject);
    if (isJackpot || payout->jackpotAward > 0) {
        _setDirection("_step_Jackpot");
    }
    if (bigwin == true &&
                    spinType != CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS &&
                    spinType != CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        _setDirection("_step_Bigwin");
    }
    if (payout->totalAward > 0) {
        _setDirection("_step_UpdateUserBalance");
    }
    if (spinResult->freespin && !superFreespinStart) {
        if (spinResult->freespin->isStart == true && spinResult->freespin->remainCount > 0) {
            _setDirection("_step_BonusIn");
        }
        else if (spinResult->freespin->remainCount == 0) {
            _setDirection("_step_BonusOut");
        }
    }
    if (superFreespinStart) {
        _setDirection("_step_SuperIn");
    }
    else if (superFreespinEnd) {
        _setDirection("_step_SuperOut");
    }
    if (bigwin == true &&
                    (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS || spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL)) {
        _setDirection("_step_Bigwin");
    }
    if (pickType > 0 && pickStart) {
        _setDirection("_step_PickIn");
    }
    else if (pickEnd && !superFreespinStart) {
        _setDirection("_step_PickOut");
    }
    else if (isPick && !superFreespin && !superFreespinStart) {
        _setDirection("_step_WaitPick");
    }
    else {
        if (bigwin == false && isRealJackpot == false && payout->totalAward <= 0) {
            _setDirection("_step_Idle");
        }
    }
    _setDirection("_step_End");
    if (isPick) {
        startDirectionProc(100);
    }
}

void CSlot346PiggyAdventure::_addStepFunc()
{
    super::_addStepFunc();

    _directionController->addStepFunction(
        "_step_Payout", () = > {
            auto spinResult = _currentResult->spinResult;
            auto payout = spinResult->payout;
            auto isJackpot = payout->isJackpot;
            auto bigwin = (payout->isBigwin&& isJackpot == false);
            if ((payout->totalAward > 0 ||
                 payout->lines.size() > 0)&& bigwin == false&& isJackpot == false) {
                _directionController->makeSmallSteps("onPayoutBeforeEffect", "onPayout");
            }
            else {
                if (bigwin || isJackpot) {
                    _directionController->makeSmallSteps("onPayoutBeforeEffect", "onPayout");
                }
                else {
                    _directionController->onNextCombineQueue();
                }
            }
        });
    _directionController->addStepFunction("_step_BonusIn",[=](){_directionController->makeSmallSteps("onStartBonusGames", "onStartBonusGames1", "onStartBonusGames2","")
        });

    _directionController->addStepFunction("_step_BonusOut",[=](){_directionController->makeSmallSteps("onEndBonusGames", "onEndBonusGames1", "onEndBonusGames2","")
        });
    _directionController->addStepFunction("_step_Bonus_Proc",[=](){_directionController->makeSmallSteps("onDelay", "freespinProc", "freespinProc1","")
        });
    _directionController->addStepFunction("_step_Bonus_Payout",[=](){_directionController->makeSmallSteps("freespinPayoutProc", "onPayoutBeforeEffect", "onDelay", "freespinPayoutProc1", "freespinPayoutProc2","")
        });
    _directionController->addStepFunction("_step_PickIn",[=](){_directionController->makeSmallSteps("onStartPickBonusGames", "onStartPickBonusGames1", "onStartPickBonusGames2", "onStartPickBonusGames3", "onWaitPick","")
        });
    _directionController->addStepFunction("_step_PickOut",[=](){_directionController->makeSmallSteps("onEndPickBonusGames", "onEndPickBonusGames1","")
        });
    _directionController->addStepFunction("_step_WaitPick",[=](){_directionController->makeSmallSteps("onWaitPick","")
        });
    _directionController->addStepFunction("_step_SuperIn",[=](){_directionController->makeSmallSteps("onStartSuperFreespin", "onStartSuperFreespin1", "onStartSuperFreespin2","")
        });
    _directionController->addStepFunction("_step_SuperOut",[=](){_directionController->makeSmallSteps("onEndSuperFreespin", "onEndSuperFreespin1", "onEndSuperFreespin2","")
        });
    _directionController->addStepFunction("_step_Pick_Proc",[=](){_directionController->makeSmallSteps("pickProc","")
        });
    _directionController->addStepFunction("_step_Pyramid_Proc",[=](){_directionController->makeSmallSteps("onDelay", "pyramidProc","")
        });
    _directionController->addStepFunction("_step_Pyramid_Payout",[=](){_directionController->makeSmallSteps("pyramidPayoutProc", "pyramidPayoutProc1", "pyramidPayoutProc2","")
        });
    _directionController->addStepFunction("_step_Oriental_Payout",[=](){_directionController->makeSmallSteps("orientalPayoutProc", "orientalPayoutProc1", "orientalPayoutProc2", "orientalPayoutProc3", "onPayoutBeforeEffect", "onDelay", "orientalPayoutProc4", "orientalPayoutProc5","")
        });
    _directionController->addStepFunction("_step_Atlantis_Proc",[=](){_directionController->makeSmallSteps("onDelay", "atlantisProc", "atlantisProc1", "atlantisProc2", "atlantisProc3", "atlantisProc4","")
        });
    _directionController->addStepFunction("_step_Atlantis_Payout",[=](){_directionController->makeSmallSteps("atlantisPayoutProc", "atlantisPayoutProc1", "atlantisPayoutProc2", "atlantisPayoutProc3","")
        });

    _directionController->addStepFunction("_step_Idle",[=](){_directionController->makeSmallSteps("onIdleSymbol","")
        });
}

void CSlot346PiggyAdventure::onDelay()
{
    _spinProcessQueue->next(500);
}

/**
 * @protected
 * 페이아웃 처리
 */
void CSlot346PiggyAdventure::onPayout(options)
{
    // 페이라인으로 전체를 보여줌
    isClick = true;
    if (_currentResult->spinResult->payout->lines) {
        showHitPaylines();
        playHidePaylineProcess();
    }
    _releaseAnimationIcons();
    // 히트 포지션의 모든 심볼에 히트 연출 발동
    _playHitAnimation();

    // 페이아웃 이펙트 처리
    auto _update_bonuspot = false;
    _bPayoutSkipping = false;
    auto _delay = 2000;
    auto _totalAward = totalAward();
    auto _duration = calcNewCoinIncreaseTime(_totalAward);
    payoutType = "1";
    playEffectSoundWithOption("snd_payout_" + payoutType, "snd_payout_" + payoutType, true);
    changeBGMVolume(0.5);
    if (isPick) {
        _delay = 0;
        _duration = calcNewCoinIncreaseTimeForPick(_totalAward);
        setOnlyBonusPotPay(true);
    }
    super::onPayout({
        delay : _delay,
        duration : _duration,
        totalAward : _totalAward,
        showWin : true,
        payoutType : !_update_bonuspot ? "update_bonuspot" : ""
    });
}

/**
 * 페이아웃 하단 이펙트 출력
 *  @protected
 */
void CSlot346PiggyAdventure::onPayoutBeforeEffect()
{
    auto award = _currentResult->spinResult->payout->totalAward;
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        award = totalAward();
    }
    auto multi = award / _totalBet;
    if (isSuperFreespin || isPick) {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            award = orientalTotalAward - orientalTotalAwardPre;
        }
        multi = award / _avgTotalBet;
    }
    auto kind = getPayoutBeforeEffectKind(multi);
    if (kind != "none") {
        m_bottomPanel->playPayoutWinEffect(kind);
        playPayoutBeforeEffectSound(kind);
        _spinProcessQueue->next(500);
    }
    else {
        _spinProcessQueue->next();
    }
}

/**
 * onPayoutCompleteValue
 */
void CSlot346PiggyAdventure::onPayoutCompleteValue(value)
{
    if (spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            auto _totalAward = value.result;
            _incremental_bonus_pot_end = _incremental_bonus_pot_end + _totalAward;
            bonuspot_number = _incremental_bonus_pot_end;
            _incremental_bonus_pot = _incremental_bonus_pot_end;
        }
        else {
            bonuspot_number = value.result;
        }
    }
    if (m_bottomPanel) {
        m_bottomPanel->playPayoutEffect();
    }
    stopEffectSound("snd_payout_" + payoutType);
    if (payoutType == "a1" || payoutType == "b1") {
        stopBGMSound();
    }
    playEffectSound("snd_payout_" + payoutType + "_end");
    changeBGMVolume(1, 1);
    super::onPayoutCompleteValue(value);
    m_bottomPanel->num_payout->setVisible(false);
}

/**
 * 슬롯 개별 오버라이드 용 잭팟 발생후에 해야할게 있으면 오버라이드 해서 사용한다.
 *
 */
void CSlot346PiggyAdventure::onJackpotEnd()
{
    pausedBGMSound(false);
    super::onJackpotEnd();
}

////////////////////////////////////////////////////////////////////////////////////////////
// 복구
////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::_onCheckBonusMode()
{
    super::_onCheckBonusMode();
}

/**
 * _onCheckBonusModeSevralProc
 * 슬롯 진입시 보너스 모드 체크하여 보너스 보전
 */
void CSlot346PiggyAdventure::_onCheckBonusModeSevralProc()
{
    _currentSpinType = _slotLoginInfo->slotState.nextSpinType;
    auto _featuredMap = getFeaturedMap();
    if (hasNextSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN)) {
        isRecovery = true;
        spinType = CSlot346PiggyAdventure.SPIN_TYPE.BONUS;
        getFeaturedInfo();
        if (_featuredMap.total_count) {
            auto _featuredInfo = _featuredMap.total_count;
            freespinTotalCount = _featuredInfo;
        }
        freespinRemainCount = _slotLoginInfo->slotState.freespinRemainCount;
        setSlotMode("bonus");
        playSlotStayForBonus();
        recoveryProcForBonus();
        setFreespinLeftcount(freespinRemainCount, freespinTotalCount);
        setOnlyBonusPotPay(false);
        m_bottomPanel->setGoodLuck();
    }
    if (hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN)) {
        if (DUG.common.settings.SLOT_HAS_TOPBAR) {
            _isAvailableClickTopbar = false;
            _topBar.checkSpinStatusUI({isRunSpin : true});
        }
        getFeaturedInfo();
        setBottomUI(false);
        if (_featuredMap.FEATURED_KEY_PICK_TYPE) {
            pickType = _featuredMap.FEATURED_KEY_PICK_TYPE;
        }
        isRecovery = true;
        spinType = pickType + 1;
        auto type = String.fromCharCode(pickType + 96);
        auto _type = "normal";

        _changeTotalBet();
        if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT) {
            superFreespinRemainCount = _featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT;
        }
        if (superFreespinRemainCount > 0) {
            isSuperFreespin = true;
            if (_featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT) {
                pickCount = _featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT;
            }
            setSuperIn(type);
            if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
                recoveryProcForPyramid();
            }
            else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
                recoveryProcForAtlantis();
            }
            else if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
                recoveryProcForOriental();
            }
        }
        else {
            if (_featuredMap.FEATURED_KEY_KEY_COUNT) {
                pickCount = _featuredMap.FEATURED_KEY_KEY_COUNT;
            }
            isPick = true;
            switch (type) {
                case "a":
                    _type = "pyramid_pick";
                    break;
                case "b":
                    _type = "atlantis_pick";
                    break;
                case "c":
                    _type = "oriental_pick";
                    break;
                default:
                    break;
            }
            setChangeBG(true, type, _type);
            makeInfoPig(true);
        }
        if (pickCount != 0) {
            makeUiKey();
        }
        setOnlyBonusPotPay(false);
        m_bottomPanel->setGoodLuck();
        setAvgTotalBet();
    }
    if (isRecovery) {
        pickStart = false;
        superFreespinStart = false;
        if (isPick) {
            if (!isControlByOther()) {
                if (DUG.common.settings.SLOT_HAS_TOPBAR) {
                    _isAvailableClickTopbar = false;
                    _topBar.checkSpinStatusUI({isRunSpin : true});
                }
                _spinProcessQueue->add(onWaitPick.bind(this));
                _spinProcessQueue->next(500);
            }
        }
        else {
            super::_onCheckBonusModeSevralProc();
        }
    }
}

void CSlot346PiggyAdventure::_onInitializedBottomUIAfter()
{
    if (hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN)) {
        setAvgTotalBet();
        if (DUG.common.settings.SLOT_HAS_TOPBAR) {
            _isAvailableClickTopbar = false;
            _topBar.checkSpinStatusUI({isRunSpin : true});
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 공통 함수(재구현)
////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::_createSlotReelWrapper(instance, options)
{
    options.onBounceStopSpin = _onBounceStopSpin.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    auto randomStart = CMiscUtils::getRandomIntInclusive(0, options.reelArray.size() - 1);
    options.startPosition = randomStart;
    options.spineContainer = _spineContainer;
    options.findIndexFunc = findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureReelWrapper(instance, options);
}

void CSlot346PiggyAdventure::totalAward(superEnd = false)
{
    auto _totalAward = _currentResult->spinResult->payout->totalAward;
    auto jackpotAward = _currentResult->spinResult->payout->jackpotAward || 0;
    auto award = _totalAward - jackpotAward;
    // if (superEnd && superTotalAward > 0) {
    // 	award = superTotalAward;
    // }
    return award;
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
void CSlot346PiggyAdventure::_onBounceStopSpin(_slotReelWrapper)
{
    const reelIndex = _slotReelWrapper->reelIndex;
    // 릴 스핀 종료 재생
    auto _resultIcons = _slotReelWrapper->getResultIcons();
    auto freespinScatterAppear = false;
    auto pickScatterAppear = false;
    auto isOn = false;
    for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
        auto _icon = _resultIcons[rowIndex];
        auto _iconId = _icon->_dug_icon_id;
        if (!isNormalScatter(_iconId)) {
            continue;
        }
        if (_iconId >= 30) {
            if (reelIndex == 2 && _pickScatterAppearCount == 0) {
                isOn = true;
            }
            else if (reelIndex == 3 && _pickScatterAppearCount <= 1) {
                isOn = true;
            }
        }
        if (_iconId < 30) {
            if (reelIndex == 2 && _bonusScatterAppearCount == 0) {
                isOn = true;
            }
            else if (reelIndex == 4 && _bonusScatterAppearCount <= 1) {
                isOn = true;
            }
        }
        if (isOn) {
            continue;
        }
        if (_iconId >= 30) {
            pickScatterAppear = true;
            _pickScatterAppearCount++;
        }
        else {
            _bonusScatterAppearCount++;
            freespinScatterAppear = true;
        }
        auto target = allReelFrame[spinType].animation;
        if (rowIndex != 0 && rowIndex != 8) {
            target = layer_reel_up;
        }
        auto _action = "appear";
        auto _aIcon = _playIconAnimation({
            icon : _icon,
            action : _action,
            loop : false,
            animationTarget : target,
            reelIndex : reelIndex,
            rowIndex : rowIndex
        });
    }
    playEffectSound("snd_reel_stop");

    if (freespinScatterAppear && !pickScatterAppear) {
        playEffectSound("snd_scatter_appear_" + _bonusScatterAppearCount);
    }
    else if (pickScatterAppear) {
        playEffectSound("snd_scatter_appear_6");
    }
}

/**
 * @protected
 * 아이콘 논히트 처리
 */
void CSlot346PiggyAdventure::_playIconNoHitAnimation(_icon, _alpha = 0.2)
{
    if (_icon->setAnimation) {
        _icon->setAnimation("on");
    }
    _icon->setOpacity(255 * _alpha);
}

void CSlot346PiggyAdventure::getFeaturedInfo()
{
    auto _featuredMap = getFeaturedMap();
    fixedPlaylineForBonus = create1DArray(_bonusReelWidth, 0);
    fixedPayoutForBonus = create1DArray(_bonusReelWidth, 0);
    fixedPlaylineForSuper = create2DArray(_reelWidth, _reelWindowHeight, 0);
    fixedPayoutForSuper = create2DArray(_reelWidth, _reelWindowHeight, 0);
    symbolPayoutForSuper = 0;
    boxResultForFreespin = [];
    if (!_featuredMap) {
        return;
    }
    if (_featuredMap.FEATURED_KEY_PICK_TYPE) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_PICK_TYPE;
        pickType = _featuredInfo;
    }
    if (_featuredMap.FEATURED_KEY_PICK_RESULT_LIST) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_PICK_RESULT_LIST;
        for (auto index = 1; index < 4; index++) {
            pickResultInfo[StringUtils::toString(index)] = _featuredInfo[index];
        }
    }
    if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT;
        superFreespinRemainCount = _featuredInfo;
        if (superFreespinRemainCount > 0 && !isSuperFreespin && !isRecovery) {
            superFreespinStart = true;
        }
    }
    if (_featuredMap.FEATURED_KEY_KEY_COUNT) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_KEY_COUNT;
        if (_featuredInfo > 0 && pickCount == 0 && !isRecovery) {
            pickCount = _featuredInfo;
            pickStart = true;
        }
    }
    if (_featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT;
        pickRemainCount = _featuredInfo;
    }

    if (_featuredMap.FEATURED_KEY_FIX_PLAYLINES) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_FIX_PLAYLINES;
        for (auto i = 0,size = _featuredInfo.size(); i < size; i++) {
            for (auto j = 0,size = _featuredInfo[i].size(); j < size; j++) {
                auto index = i * 9 + j + 1;
                fixedPlaylineForBonus[index] = _featuredInfo[i][j];
            }
        }
    }
    if (_featuredMap.FEATURED_KEY_FIX_PAYOUTLINES) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_FIX_PAYOUTLINES;
        for (auto i = 0,size = _featuredInfo.size(); i < size; i++) {
            for (auto j = 0,size = _featuredInfo[i].size(); j < size; j++) {
                auto index = i * 9 + j + 1;
                fixedPayoutForBonus[index] = _featuredInfo[i][j];
            }
        }
    }
    if (_featuredMap.FEATURED_KEY_DIG_HEIGHT_LIST) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_DIG_HEIGHT_LIST;
        for (auto i = 0,size = _featuredInfo.size(); i < size; i++) {
            digHeightInfo[i] = _featuredInfo[i];
        }
    }
    if (_featuredMap.FEATURED_KEY_CHEST_LIST) {
        auto _featuredInfo = _featuredMap.FEATURED_KEY_CHEST_LIST;
        for (auto i = 0,size = _featuredInfo.size(); i < size; i++) {
            boxResultForFreespin[i] = _featuredInfo[i];
        }
    }
    // if (_featuredMap.total_count) {
    // 	auto _featuredInfo = _featuredMap.total_count;
    // 	freespinTotalCount = _featuredInfo;
    // }
    if (isSuperFreespin) {
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            if (_featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES) {
                auto _featuredInfo = _featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES;
                for (auto i = 0,size = _featuredInfo.size(); i < size; i++) {
                    for (auto j = 0,size = _featuredInfo[i].size(); j < size; j++) {
                        fixedPlaylineForSuper[i][j] = _featuredInfo[i][j];
                    }
                }
            }
            if (_featuredMap.FEATURED_KEY_FIX_PAYOUTLINES) {
                auto _featuredInfo = _featuredMap.FEATURED_KEY_FIX_PAYOUTLINES;
                for (auto i = 0,size = _featuredInfo.size(); i < size; i++) {
                    for (auto j = 0,size = _featuredInfo[i].size(); j < size; j++) {
                        fixedPayoutForSuper[i][j] = _featuredInfo[i][j];
                    }
                }
            }
            if (_featuredMap.FEATURED_KEY_SUPER_SYMBOL_PAYOUT) {
                auto _featuredInfo = _featuredMap.FEATURED_KEY_SUPER_SYMBOL_PAYOUT;
                symbolPayoutForSuper = _featuredInfo;
            }
        }
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        if (_featuredMap.respin_status) {
            auto _featuredInfo = _featuredMap.respin_status;
            spinStatusForAtlantis = _featuredInfo;
        }
        if (_featuredMap.respin_award) {
            auto _featuredInfo = _featuredMap.respin_award;
            respinAwardForAtlantis = _featuredInfo;
        }
        if (_featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES) {
            auto _featuredInfo = _featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES;
            for (auto i = 0,size = _featuredInfo.size(); i < size; i++) {
                for (auto j = 0,size = _featuredInfo[i].size(); j < size; j++) {
                    fixedPlaylineForSuper[i][j] = _featuredInfo[i][j];
                }
            }
        }
    }
    if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        if (_featuredMap.feature) {
            auto _featuredInfo = _featuredMap.feature;
            for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
                for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
                    if (_featuredInfo[reelIndex][rowIndex]) {
                        fixedPayoutForSuper[reelIndex][rowIndex] = _featuredInfo[reelIndex][rowIndex];
                    }
                }
            }
        }
        if (_featuredMap.respin_payout) {
            superTotalAward = _featuredMap.respin_payout;
        }
    }
}

/**
 * 슬롯 모드를 설정한다.
 */
void CSlot346PiggyAdventure::setSlotMode(type)
{
    if (m_bottomPanel) {
        m_bottomPanel->setMode(type);
    }
    checkButtonStatus();
    setReelVisible(spinType);
    if (type == "bonus") {
        setReelArrayToReel(1);
        if (spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            bg->gotoAndStop("bonus");
            reel_bg->gotoAndStop("bonus");
        }
        bonuspot_number = 0;
        removeUiScroll();
    }
    else {
        setReelArrayToReel(0);
        payout_number = 0;
        m_bottomPanel->area_payout->setVisible(false);
        bg->gotoAndStop("normal");
        reel_bg->gotoAndStop("normal");
    }
}

set jackpot_number(value)
{
    super::jackpot_number = value;
    setMultiPotText();
}

void CSlot346PiggyAdventure::_changeTotalBet()
{
    if (_customFreespinInfo) {
        return;
    }
    super::_changeTotalBet();
    auto _featuredMap = getFeaturedMap();
    if (_featuredMap && _featuredMap.aveTB) {
        auto _featuredInfo = _featuredMap.aveTB;
        _avgTotalBet = _featuredInfo;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 공통 함수
////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::setAvgTotalBet()
{
    m_bottomPanel->num_totalbet->setString("AVG")
    m_bottomPanel->num_totalbet->setAnchorPoint(ccp(0.5, 0.5));
}

void CSlot346PiggyAdventure::setBottomUI(bool)
{
    m_bottomPanel->btn_spin.interactive = bool;
    m_bottomPanel->btn_maxbet.interactive = bool;
    m_bottomPanel->btn_betplus.interactive = bool;
    m_bottomPanel->btn_betminus.interactive = bool;
    if (bool) {
        m_bottomPanel->setEnableBetBtn();
        m_bottomPanel->setEnableMaxBet();
        m_bottomPanel->setEnableSpinBtn();
    }
    else {
        m_bottomPanel->setDisableBetBtn();
        m_bottomPanel->setDisableMaxBet();
        m_bottomPanel->setDisableSpinBtn();
    }
}

void CSlot346PiggyAdventure::setBonustime(visible)
{
    m_bottomPanel->setBonustimeBtn(visible);
    if (isFreespin) {
        m_bottomPanel->area_skip->setVisible(true);
    }
    m_bottomPanel->btn_spin->setVisible(!visible);
    m_bottomPanel->btn_spinleft->setVisible(!visible);
    setBottomUI(!visible);
}

/**
 * 프리스핀 잔여횟수를 설정한다.
 *  @protected
 * @param {number} leftcount - free spin left count
 */
void CSlot346PiggyAdventure::setFreespinLeftcount(leftcount, totalCount)
{
    if (m_bottomPanel) {
        m_bottomPanel->setSpinsLeft(leftcount, false, totalCount);
    }
    else if (bg.number_spinsleft && bg.text_spinleft) {
        bg.number_spinsleft._text->setString(leftcount)
        bg.text_spinleft->gotoAndStop(leftcount != 1 ? "spins" : "spin");
    }
}
/**
 * 스핀 시 차감 정보 작성
 * @protected
 */
void CSlot346PiggyAdventure::_setSpinDeductions()
{
    // 금액 차감
    if (isNextSpinNoMoneyIn() == false) {
        mychips_number = mychips_number - _totalBet;
    }
}

void CSlot346PiggyAdventure::playEffectSpine(spineName, _parent, action)
{
    auto spine = _spineContainer->borrow(spineName);
    _parent->addChild(spine);
    spine->setAnimation(action);
    spine->addAnimationComplete(action,[this](string entry) {
            coFunctionWait(0, [this] {
                    if (spine) {
                        spine->removeFromParent();
                        spine = nullptr;
                    }
                });
        });
}

// z값으로 정렬
void CSlot346PiggyAdventure::depthCompare(a, b)
{
    if (a.z == undefined) {
        a.z = 0;
    }
    if (b.z == undefined) {
        b.z = 0;
    }

    if (a.z < b.z) {
        return -1;
    }

    if (a.z > b.z) {
        return 1;
    }
    return 0;
}

void CSlot346PiggyAdventure::getSlotLocalPosision(_spine, slotName)
{
    auto bone = _spine.getBone(slotName);
    if (bone) {
        return [ bone.ax, bone.ay ];
    }
    else {
        return [ 0, 0 ];
    }
}

/**
 * 페이아웃 증가연출
 *  @deprecated
 *  @protected
 *  @param {Object} options - complete callback 등.
 */
void CSlot346PiggyAdventure::playPayoutIncrement(options)
{
    _timeline_payout->timeScale(1);
    auto _value = {value : 0, init : options.init, result : options.result, options : options};
    _timeline_payout->to(_value, options.duration, {
        value : 1,
        onUpdate : onPayoutUpdateValueBindMe ? onPayoutUpdateValueBindMe : onPayoutUpdateValueBindMe = onPayoutUpdateValue.bind(this),
        onUpdateParams : [_value],
        onComplete : onPayoutCompleteValueBindMe ? onPayoutCompleteValueBindMe : onPayoutCompleteValueBindMe = onPayoutCompleteValue.bind(this),
        onCompleteParams : [ _value, options.onComplete ],
    });
}

void CSlot346PiggyAdventure::setOnlyBonusPotPay(visible)
{
    if (visible) {
        _incremental_bonus_pot = 0;
        _incremental_bonus_pot_end = 0;
    }
    bonuspot_number = 0;
    _bUse_update_bonus_pot = visible;
    m_bottomPanel->num_bonuspot->setVisible(visible);
    m_bottomPanel->num_payout->setVisible(false);
}

void CSlot346PiggyAdventure::_initAllAnimations()
{
    _setResultIconsAlpha(1);
    hideAllPaylines();
    _releaseAllReelAnimation();
    _releaseAnimationIcons();
    if (removeHitBoxFlag) {
        removeHitBoxForNormal();
        removeHitBoxFlag = false;
    }
}

void CSlot346PiggyAdventure::create1DArray(columns, value)
{
    auto arr = [];
    for (auto i = 0; i < columns; i++) {
        arr[i] = value;
    }
    return arr;
}

void CSlot346PiggyAdventure::create2DArray(columns, rows, value)
{
    auto arr = [];
    for (auto i = 0; i < columns; i++) {
        arr[i] = [];
        for (auto j = 0; j < rows; j++) {
            arr[i][j] = value;
        }
    }
    return arr;
}

void CSlot346PiggyAdventure::create3DArray(columns, rows, num, value)
{
    auto arr = [];
    for (auto i = 0; i < columns; i++) {
        arr[i] = [];
        for (auto j = 0; j < rows; j++) {
            arr[i][j] = [];
            for (auto x = 0; x < num; x++) {
                arr[i][j][x] = value;
            }
        }
    }
    return arr;
}

void CSlot346PiggyAdventure::hideNeedlessIcons()
{
    for (auto reelIndex = 0; reelIndex < _reelWidth; reelIndex++) {
        auto _slotReelWrapper = slotReels[reelIndex];
        for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
            _slotReelWrapper->hideNeedlessIcon();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// 페이테이블
////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::_createPaytable()
{
    _paytable = new CAnimatePaytable({
        type : _slotConfig.slot_type,
        payouts : _slotInfo->payoutArray,
        setSymbolPayout : setSymbolPayoutCustomBindMe ? setSymbolPayoutCustomBindMe : setSymbolPayoutCustomBindMe = setSymbolPayoutCustom.bind(this)
    });

    super::_createPaytable(_paytable);
}

void CSlot346PiggyAdventure::setSymbolPayoutCustom(options)
{
    auto _number = options.unitBet * options.payouts[options.symbolID][options.hitcount];

    options.text->setString(options.hitcount + " - " + CMiscUtils::abbreviatesNumberOption({num : _number}))
}

/**
 * 페이테이블 블록레이어 생성
 */
void CSlot346PiggyAdventure::_createPaytableBlocklayer()
{
    _paytable_block_layer = new PIXI.Graphics();
    _paytable_block_layer.beginFill(0x000000);
    _paytable_block_layer.drawRect(0, 0, DUG.common.settings.RENDERER_WIDTH, DUG.common.settings.RENDERER_HEIGHT);
    _paytable_block_layer.endFill();
    _paytable_block_layer.interactive = true;
    _paytable_block_layer->setOpacity(255 * 0.7);

    addChild(_paytable_block_layer);
}

////////////////////////////////////////////////////////////////////////////////////////////
// 페이 라인
////////////////////////////////////////////////////////////////////////////////////////////
/**
 * 페이라인 정의
 * @override
 */
void CSlot346PiggyAdventure::_getPayLines()
{
    auto paylines = [
        [ 0, 0, 0, 0, 0 ], [ 1, 1, 1, 1, 1 ], [ 2, 2, 2, 2, 2 ], [ 3, 3, 3, 3, 3 ], [ 4, 4, 4, 4, 4 ],
        [ 5, 5, 5, 5, 5 ], [ 6, 6, 6, 6, 6 ], [ 7, 7, 7, 7, 7 ], [ 8, 8, 8, 8, 8 ], [ 0, 1, 1, 1, 0 ],
        [ 1, 2, 2, 2, 1 ], [ 2, 3, 3, 3, 2 ], [ 3, 4, 4, 4, 3 ], [ 4, 5, 5, 5, 4 ], [ 5, 6, 6, 6, 5 ],
        [ 6, 7, 7, 7, 6 ], [ 7, 8, 8, 8, 7 ], [ 1, 0, 0, 0, 1 ], [ 2, 1, 1, 1, 2 ], [ 3, 2, 2, 2, 3 ],
        [ 4, 3, 3, 3, 4 ], [ 5, 4, 4, 4, 5 ], [ 6, 5, 5, 5, 6 ], [ 7, 6, 6, 6, 7 ], [ 8, 7, 7, 7, 8 ],
        [ 0, 1, 0, 1, 0 ], [ 1, 2, 1, 2, 1 ], [ 2, 3, 2, 3, 2 ], [ 3, 4, 3, 4, 3 ], [ 4, 5, 4, 5, 4 ],
        [ 5, 6, 5, 6, 5 ], [ 6, 7, 6, 7, 6 ], [ 7, 8, 7, 8, 7 ], [ 1, 0, 1, 0, 1 ], [ 2, 1, 2, 1, 2 ],
        [ 3, 2, 3, 2, 3 ], [ 4, 3, 4, 3, 4 ], [ 5, 4, 5, 4, 5 ], [ 6, 5, 6, 5, 6 ], [ 7, 6, 7, 6, 7 ],
        [ 8, 7, 8, 7, 8 ], [ 0, 1, 2, 1, 0 ], [ 1, 2, 3, 2, 1 ], [ 2, 3, 4, 3, 2 ], [ 3, 4, 5, 4, 3 ],
        [ 4, 5, 6, 5, 4 ], [ 5, 6, 7, 6, 5 ], [ 6, 7, 8, 7, 6 ], [ 2, 1, 0, 1, 2 ], [ 3, 2, 1, 2, 3 ],
        [ 4, 3, 2, 3, 4 ], [ 5, 4, 3, 4, 5 ], [ 6, 5, 4, 5, 6 ], [ 7, 6, 5, 6, 7 ], [ 8, 7, 6, 7, 8 ],
        [ 1, 2, 1, 0, 1 ], [ 2, 3, 2, 1, 2 ], [ 3, 4, 3, 2, 3 ], [ 4, 5, 4, 3, 4 ], [ 5, 6, 5, 4, 5 ],
        [ 6, 7, 6, 5, 6 ], [ 7, 8, 7, 6, 7 ], [ 1, 0, 1, 2, 1 ], [ 2, 1, 2, 3, 2 ], [ 3, 2, 3, 4, 3 ],
        [ 4, 3, 4, 5, 4 ], [ 5, 4, 5, 6, 5 ], [ 6, 5, 6, 7, 6 ], [ 7, 6, 7, 8, 7 ], [ 1, 1, 2, 1, 1 ],
        [ 2, 2, 3, 2, 2 ], [ 3, 3, 4, 3, 3 ], [ 4, 4, 5, 4, 4 ], [ 5, 5, 6, 5, 5 ], [ 6, 6, 7, 6, 6 ],
        [ 7, 7, 8, 7, 7 ], [ 0, 0, 1, 2, 3 ], [ 1, 1, 2, 3, 4 ], [ 2, 2, 3, 4, 5 ], [ 3, 3, 4, 5, 6 ],
        [ 4, 4, 5, 6, 7 ], [ 5, 5, 6, 7, 8 ], [ 3, 2, 1, 0, 0 ], [ 4, 3, 2, 1, 1 ], [ 5, 4, 3, 2, 2 ],
        [ 6, 5, 4, 3, 3 ], [ 7, 6, 5, 4, 4 ], [ 8, 7, 6, 5, 5 ], [ 0, 1, 2, 3, 3 ], [ 1, 2, 3, 4, 4 ],
        [ 2, 3, 4, 5, 5 ], [ 3, 4, 5, 6, 6 ], [ 4, 5, 6, 7, 7 ], [ 5, 6, 7, 8, 8 ], [ 3, 3, 2, 1, 0 ],
        [ 4, 4, 3, 2, 1 ], [ 5, 5, 4, 3, 2 ], [ 6, 6, 5, 4, 3 ], [ 7, 7, 6, 5, 4 ], [ 8, 8, 7, 6, 5 ]
    ];

    return paylines;
}

/**
 * 페이라인 그려줌
 */
void CSlot346PiggyAdventure::_setWinLines()
{
    auto borderColors = options.paylines.borderColors || [0x3333DD];
    auto borderWidth = options.paylines.borderWidth || 6;
    auto borderAlpha = options.paylines.borderAlpha || 1;
    auto innerColors = options.paylines.innerColors || [0xFFFFFF];
    auto innerWidth = options.paylines.innerWidth || 0;
    auto innerAlpha = options.paylines.innerAlpha || 1;

    auto splitLines = options.paylines.splitLines || false;

    auto plusLength = options.paylines.sidePlusLength || 0;

    auto _paylineArr = _getPayLines();
    if (!_paylineArr) {
        DUG.Log("_getPayLines 함수가 정의되지 않음");
        return;
    }
    auto usingCheck = [];
    auto unitLineCount = parseInt(_lineCount / _reelWindowHeight) * 2 + 1;

    if (splitLines == true) {
        _readyWinlineSplit(usingCheck, unitLineCount);
    }
    for (auto i = 0, length = _lineCount; i < length; ++i) {
        auto positions = [];
        auto fixedRow = [];
        auto usingIndex = 0;
        for (auto reelIndex = -1; reelIndex < _reelWidth + 1; reelIndex++) {
            auto addValue = 0;
            auto x = reelIndex;
            if (reelIndex < 0) {
                x = 0;
                addValue = (_iconWidth + plusLength) * -0.5;
            }
            else if (reelIndex == _reelWidth) {
                x = _reelWidth - 1;
                addValue = (_iconWidth + plusLength) * 0.5;
            }
            auto y = _paylineArr[i][x];
            if (splitLines == true) {
                if (_checkStraightLine(reelIndex, _paylineArr, i, x, y) == true) {
                    continue;
                }
            }
            auto slotReelWrapper = slotReels[x];
            auto _resultIcons = slotReelWrapper->getResultIcons();
            auto _icon = _resultIcons[y];

            auto resultY = layer_reel_frame->getPositionY() + (_iconHeight * y) + (_iconHeight * 0.5);
            if (splitLines == true) {
                resultY = _checkOverlapLine(unitLineCount, fixedRow, usingCheck, y, resultY, usingIndex, i);
            }
            usingIndex++;
            auto _reel = layer_reel_frame[StringUtils::format("reel_%d", x + 1 )];
            positions.push_back({x : layer_reel_frame->getPositionX() + _reel->getPositionX() + addValue, y : resultY});
        }
        if (payline[StringUtils::format("betline%d", i + 1 )]) {
            payline[StringUtils::format("betline%d", i + 1 )]);
            delete payline[StringUtils::format("betline%d", i + 1 )];
        }
        if (!payline[StringUtils::format("betline%d", i + 1 )]) {
            payline[StringUtils::format("betline%d", i + 1 )] = new PIXI.Container();
            payline->addChild(payline[StringUtils::format("betline%d", i + 1 )]);
        }
        payline[StringUtils::format("betline%d", i + 1 )]->setPositionX(0);
        payline[StringUtils::format("betline%d", i + 1 )]->setPositionY(0);
        if (borderWidth > 0.0) {
            auto line = new PIXI.Graphics();
            auto color = borderColors.shift();
            line.lineStyle(borderWidth, color, borderAlpha);
            _drawWinLine(line, positions);
            borderColors.push_back(color);
            payline[StringUtils::format("betline%d", i + 1 )]->addChild(line);
        }
        if (innerWidth > 0.0) {
            auto line2 = new PIXI.Graphics();
            auto color = innerColors.shift();
            line2.lineStyle(innerWidth, color, innerAlpha);
            _drawWinLine(line2, positions);
            innerColors.push_back(color);
            payline[StringUtils::format("betline%d", i + 1 )]->addChild(line2);
        }
        //기존 윈라인 처리시 호출되는 함수
        payline[StringUtils::format("betline%d", i + 1 )]->gotoAndStop = () = > {};
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// direction 공통 함수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlot346PiggyAdventure::_setDirections()
{
    auto spinResult = _currentResult->spinResult;
    auto payout = spinResult->payout;
    bool _isRealJackpot = isRealJackpot();
    bool bigwin = (payout->isBigwin && _isRealJackpot == false);
    // auto _featuredMap = getFeaturedMap();
    if (payout->totalAward > 0 ||
        (payout->lines.size() > 0)) {
        _setDirection("_step_Payout");
    }
    bool isJackpot = (payout->isJackpot);
    if (isJackpot || payout->jackpotAward > 0) {
        _setDirection("_step_Jackpot");
    }

    if (bigwin == true) {
        _setDirection("_step_Bigwin");
    }
    if (payout->totalAward > 0) {
        _setDirection("_step_UpdateUserBalance");
    }

    if (hasSpinType(CAnimateSlotBase::SPIN_MODE::FREESPIN) == false) {
        if (spinResult->freespin && spinResult->freespin->isStart == true && spinResult->freespin->remainCount > 0) {
            _setDirection("_step_BonusIn");
        }
    }
    else {
        if (spinResult->freespin && spinResult->freespin->isRetrigger == true) {
            _setDirection("_step_BonusIn");
        }

        //_setDirection("_step_Featured01");
        if (spinResult->freespin->remainCount == 0 &&
            (hasNextSpinType(CAnimateSlotBase::SPIN_MODE::FREESPIN) == false) &&
            (hasNextSpinType(CAnimateSlotBase::SPIN_MODE::RESPIN) == false)) {  // bonus end

            //_setDirection("_step_Featured02");

            _setDirection("_step_BonusOut");
        }
    }
    //_setDirection("_step_AddLink");
    _setDirection("_step_End");
}

void CSlot346PiggyAdventure::_defineSmallStepFunc()
{
    super::_defineSmallStepFunc();
    setFunction("onPayout", CC_CALLBACK_0(CSlot346PiggyAdventure::onPayout, this));
    setFunction("onPayoutBeforeEffect", CC_CALLBACK_0(CSlot346PiggyAdventure::onPayoutBeforeEffect, this));
    // setFunction("onStartBonusGames1", CC_CALLBACK_0(CSlot346PiggyAdventure::onStartBonusGames1, this));
    // setFunction("onStartBonusGames2", CC_CALLBACK_0(CSlot346PiggyAdventure::onStartBonusGames2, this));
    // setFunction("onEndBonusGames1", CC_CALLBACK_0(CSlot346PiggyAdventure::onEndBonusGames1, this));
    // setFunction("onEndBonusGames2", CC_CALLBACK_0(CSlot346PiggyAdventure::onEndBonusGames2, this));
    setFunction("onDelay", CC_CALLBACK_0(CSlot346PiggyAdventure::onDelay, this));
}

void CSlot346PiggyAdventure::_addStepFunc()
{
    auto _directionController = getVariable<CDirectionController*>("CDirectionController");
    super::_addStepFunc();
    _directionController->addStepFunc("_step_Payout", [=]() {
        _directionController->makeSmallSteps("onPayoutBeforeEffect", "onPayout", "");
    });
    _directionController->addStepFunc("_step_BonusIn", [=]() {
        _directionController->makeSmallSteps("onStartBonusGames", "onStartBonusGames1", "onStartBonusGames2", "");
    });
    _directionController->addStepFunc("_step_BonusOut", [=]() {
        _directionController->makeSmallSteps("onEndBonusGames", "onEndBonusGames1", "onEndBonusGames2", "");
    });
    _directionController->addStepFunc("_step_Bigwin", [=]() {
        _directionController->makeSmallSteps("onBigwinBefore", "onBigwin", "onBigwinEnd", "");
    });
}

void CSlot346PiggyAdventure::onDelay()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CMiscFunc::PurgeMemoryLite();
    _spinProcessQueue->next(700);
}

void CSlot346PiggyAdventure::onPayout()
{
    auto payout = _currentResult->spinResult->payout;
    // 페이라인으로 전체를 보여줌
    if (payout->lines.size() > 0) {
        showHitPaylines();
    }
    _releaseAnimationIcons();
    // 히트 포지션의 모든 심볼에 히트 연출 발동
    _playHitAnimation();
    // 페이아웃 이펙트 처리
    auto _update_bonuspot = false;
    auto _totalAward = payout->totalAward;
    auto _duration = calcCoinIncreaseTime(_totalAward);
    playEffectSoundWithOption("snd_payout_1", "snd_payout_1", true);
    changeBGMVolume(0.2f);
    CAnimateSlotBase::onPayout_options options;
    options.duration = _duration;
    options.totalAward = _totalAward;
    options.nextQueueDelay = 2.0f;
    options.payoutType = !_update_bonuspot ? "update_bonuspot" : "";
    super::onPayout(options);
}
/**
 * onPayoutCompleteValue
 */
void CSlot346PiggyAdventure::onPayoutCompleteValue()
{
    if (m_bottomPanel) {
        m_bottomPanel->PlayPayoutEffect();
    }
    if (hasSpinType(CAnimateSlotBase::SPIN_MODE::RESPIN)) {
        stopEffectSound(StringUtils::format("snd_payout_%d", payoutType));
        playEffectSound(StringUtils::format("snd_payout_%d_end", payoutType));
    }
    else {
        stopEffectSound("snd_payout_1");
        playEffectSound("snd_payout_1_end");
        changeBGMVolume(1, 1);
    }
    // 보너스 팟 공통화
    if (hasSpinType(CAnimateSlotBase::SPIN_MODE::RESPIN)) {
        long long _totalAward = m_Payout_totalAward;
        _incremental_bonus_pot_end = _totalAward;
        set_bonuspot_number(_incremental_bonus_pot_end);
        _incremental_bonus_pot = _incremental_bonus_pot_end;
    }
    super::onPayoutCompleteValue();
}

void CSlot346PiggyAdventure::onPayoutBeforeEffect()
{
    auto payout = _currentResult->spinResult->payout;
    long long award = payout->totalAward;
    if (award > 0) {
        int multi = (int)(award / _totalBet);
        auto kind = getPayoutBeforeEffectKind(multi);
        if (kind != CAnimateBottomUI::PAYOUT_TEXT::NONE) {
            ((CGameNewBottomInterface*)m_bottomPanel)->playPayoutWinEffect((int)kind);
            playPayoutBeforeEffectSound(kind);
            _spinProcessQueue->next(500);
        }
        else {
            _spinProcessQueue->next();
        }
    }
    else {
        _spinProcessQueue->next();
    }
}

/**
 * 빅윈 팝업 종료 후 처리
 *
 * @protected
 */
void CSlot346PiggyAdventure::onBigwinEnd()
{
    // 보너스 모드시
    if (hasSpinType(CAnimateSlotBase::SPIN_MODE::FREESPIN)) {
        set_bonuspot_number(_currentResult->spinResult->freespin->totalAward);

        // 보너스 팟 공통화 빅윈 end 시 보너스팟 갱신.
        if (_bUse_update_bonus_pot == true) {
            _incremental_bonus_pot = _incremental_bonus_pot_end = _currentResult->spinResult->freespin->totalAward;
        }
    }
    _spinProcessQueue->next(_delayAfterBigwin());
}

void CSlot346PiggyAdventure::onBigwinEndBGMControl()
{
    auto popup = dynamic_cast<CNewBigWinPopup*>(removeVariable("bigwin_popup"));
    if (popup) {
        changeBGMVolume(1);
        // CSoundBase::sharedInstance()->PlayBackGroundMusic(popup->m_PrevBGMPath.c_str());
    }
}

void CSlot346PiggyAdventure::onJackpotEnd()
{
    showHitPaylines();
    _playHitAnimation();
    super::onJackpotEnd();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///복구
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlot338HooHooHooMonkey::_onCheckBonusMode()
{
    super::_onCheckBonusMode();
}

void CSlot338HooHooHooMonkey::_onCheckBonusModeSevralProc()
{
    auto slotState = getSlotState();
    if (slotState) {
        auto _featuredMap = getFeaturedMap();
        _currentSpinType = slotState->nextSpinType;
        if (hasSpinType(CAnimateSlotBase::SPIN_MODE::FREESPIN) == false && hasSpinType(CAnimateSlotBase::SPIN_MODE::RESPIN) == false) {
            return;
        }
        super::_onCheckBonusModeSevralProc();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///공통 함수(재구현)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSlotReelWrapper* CSlot346PiggyAdventure::_createSlotReelWrapper(CReelFrame* instance, SlotReelWrapper::Options options)
{
    int randomStart = CMiscUtils::getRandomIntInclusive(0, options.reelArray.size() - 1);

    options.startPosition = randomStart;
    options.reelArray = _setReelRandomSymbol(0, options.reelArray);
    options.findIndexFunc = CC_CALLBACK_0(CSlot346PiggyAdventure::findIndexFunc, this);
    options.onBounceStopSpin = CC_CALLBACK_1(CSlot346PiggyAdventure::onBounceStopSpin, this);
    CSlot346PiggyAdventureReelWrapper::SlotInfo info;
    info.slot = this;
    auto reelWrapper = CSlot346PiggyAdventureReelWrapper::create(instance, options, info);
    instance->addChild(reelWrapper);
    return reelWrapper;
}

void CSlot346PiggyAdventure::onBounceStopSpin(CSlotReelWrapper* _slotReelWrapper)
{
    auto reelIndex = _slotReelWrapper->reelIndex();
    auto _resultIcons = _slotReelWrapper->getResultIcons();
    auto scatterAppear = false;
    auto monkeyScatterAppear = false;
    auto respinScatterAppear = false;
    auto freespinScatterAppear = false;
    for (auto rowIndex = 0; rowIndex < _reelWindowHeight; rowIndex++) {
        auto _icon = _resultIcons[rowIndex];
        auto _iconId = _icon->_dug_icon_id;
        if (_iconId == 2) {
            _bonusScatterAppearCount++;
            freespinScatterAppear = true;
        }
        // CCNode* _animationTarget = nullptrptrptrptrptr;
        // auto _aniName = "hit";
        CAnimateSlotBase::playIcon_options options(_icon, "appear", false);
        // options.aniName = _aniName;
        // options.animationTarget = _animationTarget;
        auto _aIcon = _playIconAnimation(options);
        auto iconInfo = SlotAddInfo::get(_aIcon);
        iconInfo->setInfo("reelIndex", reelIndex);
        iconInfo->setInfo("rowIndex", rowIndex);
    }
    if (!_bSpinSkipping) {
        if (scatterAppear && !monkeyScatterAppear && !respinScatterAppear && !freespinScatterAppear) {
            playEffectSound("scatterAppear");
        }
        else if (monkeyScatterAppear && !respinScatterAppear && !freespinScatterAppear) {
            playEffectSound("monkeyScatterAppear");
        }
        else if (respinScatterAppear && !freespinScatterAppear) {
            playEffectSound("respinScatterAppear");
        }
        else if (freespinScatterAppear) {
            playEffectSound(StringUtils::format("freespinScatterAppear_%d", _bonusScatterAppearCount));
        }
    }
    if (_bSpinSkipping && !_skip && (monkeyScatterAppear || scatterAppear || respinScatterAppear || freespinScatterAppear)) {
        if (scatterAppear && !monkeyScatterAppear && !respinScatterAppear && !freespinScatterAppear) {
            playEffectSound("scatterAppear");
        }
        else if (monkeyScatterAppear && !respinScatterAppear && !freespinScatterAppear) {
            playEffectSound("monkeyScatterAppear");
        }
        else if (respinScatterAppear && !freespinScatterAppear) {
            playEffectSound("respinScatterAppear");
        }
        else if (freespinScatterAppear) {
            playEffectSound(StringUtils::format("freespinScatterAppear_%d", _bonusScatterAppearCount));
        }
        _skip = true;
    }
}
void CSlot346PiggyAdventure::_onChangeJackpotParty(SlotInfo* slotInfo)
{
    DLOG("CAnimateSlotBase::_onChangeJackpotParty : %s", slotInfo->isJackpotParty ? "true" : "false");

    // 기존 상태와 다르다면
    //    if (currentJackpotParty != slotInfo->isJackpotParty) {
    //        currentJackpotParty = slotInfo->isJackpotParty;
    //        if (currentJackpotParty == true) {
    //            jackpotParty->setVisible(true);
    //        }
    //        else {
    //            jackpotParty->setVisible(false);
    //        }
    //    }
}

void CSlot346PiggyAdventure::_setWinLines()
{
    mWinlineOptions.innerWidth = 2;
    mWinlineOptions.borderWidth = 5;
    CSlotBase::_setWinLines();
}

int CSlot346PiggyAdventure::SetJackpotValue()
{
    int retVal = 0;

    retVal = CSlotServerBase::SetJackpotValue();

    return retVal;
}

void CSlot346PiggyAdventure::getFeaturedInfo()
{
    auto _featuredMap = getFeaturedMap();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///공통 함수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @protected
 */
string CSlot338HooHooHooMonkey::convertPayout(double value, int max)
{
    vector<string> str = {"K", "M", "B"};
    auto result = value;
    int i, size;
    for (i = 0, size = (int)str.size(); i < size; i++) {
        result = result / 1000;
        if (result < 999) {
            break;
        }
    }
    string _result = StringUtils::toString(result);
    if (_result.size()() > max) {
        if (_result.find(".") == max - 1)
            _result = _result.substr(0, max - 1);
        else
            _result = _result.substr(0, max);
    }
    return _result + str[i];
}

void CSlot346PiggyAdventure::_playIconNoHitAnimation(CSlotIconSpine* icon, GLubyte alpha)
{
    string timelineName = "Default Timeline";
    if (icon->hasAnimation("on")) {
        timelineName = "on";
    }
    icon->setAnimation(timelineName);
    icon->setOpacity(alpha);
    auto addIcon = icon->getIcon()->getVariable<CSimpleCCBI*>("addIcon");
    if (addIcon) {
        addIcon->setOpacity(alpha);
    }
}

void CSlot346PiggyAdventure::onAnimatedSpinStop(CReelFrame* _reel, int delay)
{
    super::onAnimatedSpinStop(_reel, delay);
    if (_spinActivereel->size() == 0) {
        stopEffectSound("spin_reel");
    }
}

vector<vector<int>> CSlot346PiggyAdventure::makeFakeReelSet(int reelIndex)
{
    int index = CMiscUtils::getRandomIntInclusive(0, 51 - 1);
    vector<int> predicArray;
    int spinType = hasSpinType(CAnimateSlotBase::SPIN_MODE::FREESPIN) ? 1 : 0;
    auto reelArray = _getReelArray(spinType);
    int _countPerReel = (int)reelArray.size() / _reelWidth;
    int startIndex = reelIndex * _countPerReel;
    int limitIndex = startIndex + _countPerReel;
    for (int i = 0; i < _reelWindowHeight; i++) {
        int symbolIndex = (startIndex + index + i);
        if (symbolIndex >= limitIndex) {
            symbolIndex = symbolIndex - _countPerReel;
        }
        predicArray.push_back(reelArray[symbolIndex]);
    }

    vector<vector<int>> temp;
    temp.push_back(predicArray);
    return temp;
}

int CSlot346PiggyAdventure::findIndexFunc()
{
    vector<int> _reelArray = _getReelArray(0);
    int _countPerReel = (int)_reelArray.size();
    return CMiscUtils::getRandomIntInclusive(0, _countPerReel - 1);
}

void CSlot346PiggyAdventure::OnSuperFreeSpinEnded()
{
    SetJackpotValue();
    CSlotBase::OnSuperFreeSpinEnded();
}

void CSlot346PiggyAdventure::OnClickBet()
{
    if (m_BetList->isVisible()) {
        //배팅메뉴 블럭레이어 가라기
        HideBetListBlackLayer();
        m_BetList->setVisible(false);
        return;
    }

    if (m_bAutoPlay || m_BonusCount > 0 || m_ReelStatus != 0 || m_BonusMode != 0 || m_RespinMode != 0) return;

    //배팅메뉴 블럭레이어 그리기
    ShowBetListBlackLayer();

    checkSoundResources("OnClickBet");

    m_BetList->ShowBetList(!m_BetList->isVisible());
    CSoundBase::sharedInstance()->StopAllEffect();
    CSoundBase::sharedInstance()->PlayInstanceSound(m_BetClickSoundPath.getCString());

    if (m_JackPotXText != nullptrptrptrptr) {
        if (m_jackpotsubSprite != nullptrptrptrptr)
            m_jackpotsubSprite->stopAllActions();

        seeJackPotBet();
    }
}

void CSlot346PiggyAdventure::_writeSlotActionLog(string category_info, string action, string action_info)
{
    /*//
     std::string type,
     std::string ip,
     std::string fbId,
     std::string category,
     std::string categoryInfo,
     std::string action,
     std::string actionInfo,
     std::string stayTime,
     std::string isPlayNow,
     std::string coin,
     std::string networkCoin
     //*/
    CIOSModule::sendActionLog("1",                                  // type
                              "",                                   // ip
                              CUserInfo::sharedInstance()->UserID,  // fbid
                              "Piggy Adventure: The Lost Empires",  // category
                              category_info,                        // category info
                              action,
                              action_info,  // action info
                              "0",          // staytime
                              "1",          // isPlayNow
                              CommonUtils::makeString("%lld", CUserInfo::sharedInstance()->Coin.number()),
                              CommonUtils::makeString("%lld", CUserInfo::sharedInstance()->NetworkCoin.number()));
}

void CSlot346PiggyAdventure::_onInitPayLine()
{
    CAnimateSlotBase::_onInitPayLine();
}

#pragma mark - 페이테이블
void CSlot346PiggyAdventure::OnPayTablePopup(CCObject* obj)
{
    if (m_bAutoPlay)
        return;
    auto paytable = CSimpleBlendCCBI::create(RM_ARP_S("piggyadventure/paytable/"), "paytable");
    CSimplePayTable* payTablePopup = CSimplePayTable::create(paytable, this, 0, 100);
    PopupManager::sharedInstance()->ShowPopup(payTablePopup, POPUP_PAYTABLE);
}

string CSlot346PiggyAdventure::getPaytableCalculate(int page, int textCount)
{
    string out = "";
    int symbolId = 0;

    if (page == 2) {
        if (textCount == 0) {
            symbolId = 100;

            for (int i = 5; i > 2; --i) {
                if (i == 5 && textCount == 0) {
                    out += "5- Jackpot";
                    out += "\n";
                    continue;
                }

                int payline[5] = {symbolId, 0, 0, 0, 0};
                out += cocos2d::CCString::createWithFormat("%d- %s", i, CMiscFunc::MakeUnitStringDot(GetPayOutForPaytable(payline, i), 3, false).c_str())->getCString();
                out += "\n";
            }
        }
        else if (textCount == 1 || textCount == 2) {
            symbolId = 100;

            for (int i = 5; i > 2; --i) {
                out += " ";

                int payline[5] = {symbolId, 0, 0, 0, 0};
                out += cocos2d::CCString::createWithFormat("%d- %s", i, CMiscFunc::MakeUnitStringDot(GetPayOutForPaytable(payline, i), 3, false).c_str())->getCString();
                out += "\n";
            }
        }
        else {
            symbolId = textCount;

            for (int i = 5; i > 2; --i) {
                out += " ";

                int payline[5] = {symbolId, 0, 0, 0, 0};
                out += cocos2d::CCString::createWithFormat("%d- %s", i, CMiscFunc::MakeUnitStringDot(GetPayOutForPaytable(payline, i), 3, false).c_str())->getCString();
                out += "\n";
            }
        }
    }

    return out;
}

void CSlot346PiggyAdventure::getPaytableTextLabel(int page, int textCount, CCLabelTTF* text)
{
    //    if (text != nullptrptrptrptr)
    //    {
    //        if (page == 1)
    //        {
    //            if (m_paytableOrgPos[textCount] == -2000)
    //            {
    //                m_paytableOrgPos[textCount] = text->getPositionY();
    //            }
    //            text->setPositionY(m_paytableOrgPos[textCount]);
    //        }
    //    }
}

long long CSlot346PiggyAdventure::GetPayOutForPaytable(int value[5], int hitcount)
{
    long long lineaward = 0;

    switch (value[0]) {
        case 1:    // jackpot
        case 100:  //와일드
        {
            if (hitcount == 2 || hitcount == 3 || hitcount == 4 || hitcount == 5) {
                lineaward = _slotLoginInfo->slotInfo->payoutArray[1][hitcount];
            }
            break;
        }
        case 3:   // high
        case 4:   // middle1
        case 5:   // middle2
        case 6:   // middle3
        case 7:   // middle4
        case 8:   // low1
        case 9:   // low2
        case 10:  // low3
        case 11:  // low4
        {
            if (hitcount == 3 || hitcount == 4 || hitcount == 5) {
                lineaward = _slotLoginInfo->slotInfo->payoutArray[value[0]][hitcount];
            }
            break;
        }
        default:
            lineaward = 0;
            break;
    }

    return lineaward * m_UnitBet.number();
}

#pragma mark - 커스텀 윈라인

int CSlot346PiggyAdventure::GetYIndexByBetLine(int betline, int x_index)
{
    return m_paylinePos[betline][x_index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DebugUI (PreRunreelProc 포함)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - 서버베이스
void CSlot346PiggyAdventure::PreRunreelProc()
{
#ifdef DEBUG_QA_TOOL
    vector<std::string> cmdArr;
    if (m_qaTool != nullptrptrptrptr) {
        stringstream extensionStream;
        extensionStream << "{";

        int qaPredictionArr[5] = {
            -1,
        };
        m_qaTool->GetQAArray(qaPredictionArr);
        if (qaPredictionArr[0] != -1) {
            char tmpStr[256] = {
                0,
            };
            sprintf(tmpStr, "\"reelSettingArray\":[%d,%d,%d,%d,%d]",
                    qaPredictionArr[0], qaPredictionArr[1], qaPredictionArr[2], qaPredictionArr[3], qaPredictionArr[4]);

            extensionStream << string(tmpStr).c_str();
            // cmdArr.push_back(string(tmpStr));

            if (testMode != -1 || testMode2 != -1 || testMode3 != -1) {
                extensionStream << ",";
            }
        }
        else if (cheat_reelArray != "") {
            extensionStream << cheat_reelArray.c_str();

            if (testMode != -1 || testMode2 != -1 || testMode3 != -1) {
                extensionStream << ",";
            }
        }

        if (testMode != -1) {
            ExtensionData::set(extensionStream, "testMode", CommonUtils::getStringMerge("", testMode), false);

            if (testMode2 != -1 || testMode3 != -1) {
                extensionStream << ",";
            }
        }

        if (testMode2 != -1) {
            ExtensionData::set(extensionStream, "testMode2", CommonUtils::getStringMerge("", testMode2), false);

            if (testMode3 != -1) {
                extensionStream << ",";
            }
        }

        if (testMode3 != -1) {
            ExtensionData::set(extensionStream, "testMode3", CommonUtils::getStringMerge("", testMode3), false);
        }
        extensionStream << "}";

        cheat_reelArray = "";
        testMode = -1;
        testMode2 = -1;
        testMode3 = -1;

        if (extensionStream.str() != "{}") {
            m_ExtensionString = extensionStream.str();
        }
    }
#endif
    onSpin();

    _bSpinResponse = false;

    CSlotServerBase::PreRunReelProc();

    // 릴상태 갱신
    SetReelStatus(CSlotStatus::REEL_SPIN);

    // clearCache();

    // 하단 스핀 수
}

#ifdef DEBUG_UI

void CSlot346PiggyAdventure::InitDebugUI()
{
    pMenu = CCMenu::create();
    pMenu->setPosition(ccp(170, 350));
    if (CUserInfo::sharedInstance()->IsResEx == true) {
        pMenu->setPosition(ccp(150, 300));
    }
    this->addChild(pMenu);

    auto mode1 = CreateMenuSpriteButton("리스핀", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode1));
    pMenu->addChild(mode1);
    auto mode2 = CreateMenuSpriteButton("멀티어레이", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode2));
    pMenu->addChild(mode2);
    auto mode3 = CreateMenuSpriteButton("망치", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode3));
    pMenu->addChild(mode3);
    auto mode4 = CreateMenuSpriteButton("미니", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode4));
    pMenu->addChild(mode4);
    auto mode5 = CreateMenuSpriteButton("마이너", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode5));
    pMenu->addChild(mode5);
    auto mode6 = CreateMenuSpriteButton("메이저", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode6));
    pMenu->addChild(mode6);
    auto mode7 = CreateMenuSpriteButton("울트라", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode7));
    pMenu->addChild(mode7);
    auto mode8 = CreateMenuSpriteButton("그랜드", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode8));
    pMenu->addChild(mode8);
    auto mode9 = CreateMenuSpriteButton("특수+망치", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode9));
    pMenu->addChild(mode9);
    auto mode10 = CreateMenuSpriteButton("드릴", CCSize(100, 35), this, menu_selector(CSlot346PiggyAdventure::OnMode10));
    pMenu->addChild(mode10);

    mode1->setVisible(false);
    mode2->setVisible(false);
    mode3->setVisible(false);
    mode4->setVisible(false);
    mode5->setVisible(false);
    mode6->setVisible(false);
    mode7->setVisible(false);
    mode8->setVisible(false);
    mode9->setVisible(false);
    mode10->setVisible(false);
    mode1->setTag(1);
    mode2->setTag(2);
    mode3->setTag(3);
    mode4->setTag(4);
    mode5->setTag(5);
    mode6->setTag(6);
    mode7->setTag(7);
    mode8->setTag(8);
    mode9->setTag(9);
    mode10->setTag(10);

    // Lastly..
    pMenu->alignItemsVertically();

    minimize = CreateMenuSpriteButton("-", CCSize(40, 40), this, menu_selector(CSlot346PiggyAdventure::OnBtnMinimize));
    minimize->setPosition(ccp(0, 250));

    maximize = CreateMenuSpriteButton("+", CCSize(40, 40), this, menu_selector(CSlot346PiggyAdventure::OnBtnMaximize));
    maximize->setPosition(ccp(0, 250));

    pMenu->addChild(minimize);
    pMenu->addChild(maximize);
}

void CSlot346PiggyAdventure::OnBtnMinimize(CCObject* pObject)
{
    if (minimize != nullptrptrptrptr) {
        minimize->setVisible(false);
        pMenu->getChildByTag(1)->setVisible(false);
        pMenu->getChildByTag(2)->setVisible(false);
        pMenu->getChildByTag(3)->setVisible(false);
        pMenu->getChildByTag(4)->setVisible(false);
        pMenu->getChildByTag(5)->setVisible(false);
        pMenu->getChildByTag(6)->setVisible(false);
        pMenu->getChildByTag(7)->setVisible(false);
        pMenu->getChildByTag(8)->setVisible(false);
        pMenu->getChildByTag(9)->setVisible(false);
        pMenu->getChildByTag(10)->setVisible(false);
    }

    if (maximize != nullptrptrptrptr) {
        maximize->setVisible(true);
    }
}

void CSlot346PiggyAdventure::OnBtnMaximize(CCObject* pObject)
{
    if (minimize != nullptrptrptrptr) {
        minimize->setVisible(true);
    }

    if (maximize != nullptrptrptrptr) {
        maximize->setVisible(false);
        pMenu->getChildByTag(1)->setVisible(true);
        pMenu->getChildByTag(2)->setVisible(true);
        pMenu->getChildByTag(3)->setVisible(true);
        pMenu->getChildByTag(4)->setVisible(true);
        pMenu->getChildByTag(5)->setVisible(true);
        pMenu->getChildByTag(6)->setVisible(true);
        pMenu->getChildByTag(7)->setVisible(true);
        pMenu->getChildByTag(8)->setVisible(true);
        pMenu->getChildByTag(9)->setVisible(true);
        pMenu->getChildByTag(10)->setVisible(true);
    }
}

std::string CSlot346PiggyAdventure::GetExtensionStringIntArray(std::string key, int arr[], int len)
{
    char tmpStr[200] = {
        0,
    };
    int idx = 0;
    idx += sprintf(tmpStr + idx, "\"%s\":[", key.c_str());
    for (int i = 0; i < len; i++) {
        idx += sprintf(tmpStr + idx, "%d", arr[i]);
        if (i < len - 1) {
            idx += sprintf(tmpStr + idx, ",");
        }
    }
    idx += sprintf(tmpStr + idx, "]");
    return std::string(tmpStr);
}
std::string CSlot346PiggyAdventure::GetExtensionStringInt(std::string key, int val)
{
    char tmpStr[200] = {
        0,
    };
    int idx = 0;
    idx += sprintf(tmpStr + idx, "\"%s\":", key.c_str());
    idx += sprintf(tmpStr + idx, "%d", val);
    return std::string(tmpStr);
}
std::string CSlot346PiggyAdventure::GetMergedExtensionString(std::vector<std::string> arr)
{
    char tmpStr[500] = {
        0,
    };
    int idx = 0;
    idx += sprintf(tmpStr + idx, "\{");
    for (int i = 0, size = arr.size(); i < size; i++) {
        idx += sprintf(tmpStr, "%s", arr[i].c_str());
        if (i < arr.size() - 1) {
            idx += sprintf(tmpStr + idx, ",");
        }
    }
    idx += sprintf(tmpStr + idx, "}");
    return std::string(tmpStr);
}
void CSlot346PiggyAdventure::OnMode1(CCObject* obj)
{
    // 프리 7
    cheat_reelArray = "\"reelSettingArray\":[0,0,0,33,33]";
}
void CSlot346PiggyAdventure::OnMode2(CCObject* obj)
{
    // 프리 10
    cheat_reelArray = "\"reelSettingArray\":[0,0,0,33,33]";
    testMode2 = 3;
}
void CSlot346PiggyAdventure::OnMode3(CCObject* obj)
{
    testMode = 400;
}
void CSlot346PiggyAdventure::OnMode4(CCObject* obj)
{
    testMode = 9;
}
void CSlot346PiggyAdventure::OnMode5(CCObject* obj)
{
    testMode = 10;
}
void CSlot346PiggyAdventure::OnMode6(CCObject* obj)
{
    testMode = 11;
}
void CSlot346PiggyAdventure::OnMode7(CCObject* obj)
{
    testMode = 12;
}
void CSlot346PiggyAdventure::OnMode8(CCObject* obj)
{
    testMode = 13;
}
void CSlot346PiggyAdventure::OnMode9(CCObject* obj)
{
    testMode = 400;
    testMode2 = 3;
}
void CSlot346PiggyAdventure::OnMode10(CCObject* obj)
{
    testMode3 = 1;
}
void CSlot346PiggyAdventure::OnMode11(CCObject* obj)
{
}
CCSprite* CSlot346PiggyAdventure::CreateColorSprite(const CCSize size, const ccColor3B color)
{
    CCSprite* blackSpr = CCSprite::create();
    blackSpr->setTextureRect(CCRect(0, 0, size.width, size.height));
    blackSpr->setColor(color);
    return blackSpr;
}
CCSprite* CSlot346PiggyAdventure::CreateLabelWithBackground(const char* label, const char* fontName, int fontSize, const CCSize size, const ccColor3B labelColor, const ccColor3B bgColor)
{
    CCSprite* blackSpr = CreateColorSprite(size, bgColor);
    auto tempLab = CCLabelTTF::create(label, fontName, fontSize);
    tempLab->setColor(labelColor);
    tempLab->setTag(1000);
    tempLab->setPosition(ccp(size.width / 2, size.height / 2));
    blackSpr->addChild(tempLab);
    return blackSpr;
}
CCMenuItem* CSlot346PiggyAdventure::CreateMenuSpriteButton(const char* name, const CCSize size, CCObject* target, SEL_MenuHandler selector)
{
    auto bgLabel = CreateLabelWithBackground(name, "Arial", 15, size, ccc3(0, 0, 0), ccc3(255, 255, 255));

    CCSprite* selectedSpr = CreateColorSprite(size, ccc3(100, 100, 100));
    return CCMenuItemSprite::create(bgLabel, selectedSpr, target, selector);
}

void CSlot346PiggyAdventure::PrintDirections()
{
    auto _dc = getVariable<CDirectionController*>("CDirectionController");

    if (_dc) {
        for (int i = 0, size = _dc->mDirections.size(); i < size; i++) {
            DLOG("directions [%d] : %s", i + 1, _dc->mDirections[i].c_str());
        }
    }
}
#endif