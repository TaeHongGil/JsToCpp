/**
 * 생성자
 * @constructor
 */
constructor(slotInfo, slotConfig)
{
    super(slotInfo, slotConfig);

    this.$IdleProcessQueue = new FunctionQueue();
    this.$hidePaylineProcessQueue = new FunctionQueue();
    this.multipots = [ "", "mini", "minor", "major", "ultra", "grand" ];
    this.multipot = [ 0, 50, 100, 150, 300, 500 ];

    this.pickInfo = {};
    //싱글
    this.slotReelsForBonus = [];
    //일반
    this.slotReelsForPyramid = [];
    //싱글
    this.slotReelsForAtlantis = [];
    //일반
    this.slotReelsForOriental = [];
    this.allReelFrame = [ this.$slot.layer_reel_frame, this.$slot.layer_reel_frame_bonus, this.$slot.layer_reel_frame_pyramid, this.$slot.layer_reel_frame_atlantis, this.$slot.layer_reel_frame_oriental ];
    this.allReels = [ this.slotReels, this.slotReelsForBonus, this.slotReelsForPyramid, this.slotReelsForAtlantis, this.slotReelsForOriental ];

    this.$fixedSymbolForBonus = [];
    this.$fixedSymbolForPyramid = [];
    this.$fixedSymbolForOriental = [];
    this.$fixedSymbolForAtlantis = [];
    this.$openBox = [];
    this.$openBoxFrame = [];
    this.$ui_multipot_text = [];
    this.$tresureBox = [];
    this.$paytalbeInfo = [ null, null, null ];
    this.$payBoxTextForAtlantis = [];
    this.$payBoxTextForAtlantisBig = [];
    this.$payHitAniForAtlantis = [];
    this.$hitboxForNormal = [];
    this.remainPickInfo = [];

    this.$payBoxNumForAtlantis = null;
    this.$payBoxNumForAtlantisBig = null;
    this.$uiScroll = null;
    this.$uiChoiceMap = null;
    this.$exitButton = null;
    this.$multipotPopup = null;
    this.$uiKey = null;
    this.$uiKeyText = null;
    this.$countDown = null;
    this.$infoPig = null;
    this.$endPopup = null;
    this.$openPopup = null;
    this.$staySpine = null;
    this.$uiTresureMap = null;
    this.$payoutSpine = null;
    this.$payoutTween = null;

    this.countDownTimeout = null;
    this.countDownTimeoutSound = null;

    this.pickResultInfo = [ {}, {}, {}, {} ];
    this.linePayForAtlantis = [];
    this.paylineInfoForAtlantis = [];
    this.openState = [];
    this.boxResultForFreespin = [];
    this.fixedPayoutForBonus = [];
    this.fixedPayoutForSuper = [];
    this.fixedPlaylineForBonus = [];
    this.fixedPlaylineForSuper = [];
    this.hitReelForOriental = [];
    this.respinAwardForAtlantis = [];
    this.fixedPayForOriental = [];
    this.digHeightInfo = [ 0, 0, 0, 0, 0 ];

    this.spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
    this._bonusReelWidth = 0;
    this.freespinRemainCount = 0;
    this.userPick = -1;
    this.pickCount = 0;
    this.pickRemainCount = 0;
    this.superFreespinRemainCount = 0;
    this.superFreespinTotalCount = 0;
    this.pickType = 0;
    this.symbolPayoutForSuper = 0;
    this.orientalCoinCount = 0;
    this.orientalMultiCount = 0;
    this.orientalTotalAward = 0;
    this.orientalTotalAwardPre = 0;
    this.spinStatusForAtlantis = 0;
    this.superTotalAward = 0;
    this.spinNum = 0;
    this.freespinTotalCount = 0;
    this._pickScatterAppearCount = 0;
    this._bonusScatterAppearCount = 0;
    this.totalPayTemp = 0;
    this._avgTotalBet = 0;

    this.payoutType = "";

    this.isSuperFreespin = false;
    this.superFreespinStart = false;
    this.superFreespinEnd = false;
    this.isPick = false;
    this.pickEnd = false;
    this.pickStart = false;
    this.isFreespin = false;
    this.openMap = false;
    this.firstSpinForSuper = false;
    this.pickRetrigger = false;
    this.removeHitBoxFlag = false;
    this.isRecovery = false;
    // 페이아웃 발생시 페이라인을 한 라인씩 그려주는 슬롯은 주석을 풀어줌
    // this._bShowSingleHitLine = true;

    this.paylinesForAtlantis = [
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
destroy(options)
{
    // 추가적으로 생성한 리소스 destroy
    super.destroy(options);
    this.$IdleProcessQueue.killAndDone();
    this.$hidePaylineProcessQueue.killAndDone();
    clearTimeout(this.countDownTimeout);
    clearTimeout(this.countDownTimeoutSound);
}

/** **************************************************************************************************
 *  [START]OVERRIDE METHODS
 *  **************************************************************************************************/

/**
 * _onInitializedInternal
 */
_onInitializedInternal()
{
    this.findIndexFunc = function(reel)
    {
        let reelLength = this._reelArray.length;
        return CMiscUtils.getRandomIntInclusive(0, reelLength - 1);
    }
    //세로모드
    if (!DUG.common.settings.SLOT_HAS_TOPBAR) {
        DUG.common.settings.RENDERER_HEIGHT = 1014;
        DUG.DoubleUCasino.pixi_app.renderer.resize(DUG.common.settings.RENDERER_WIDTH, DUG.common.settings.RENDERER_HEIGHT);
    }
    super._onInitializedInternal();
    //스캐터 등장한 릴을 위로 올리기 위함
    for (let index = 0; index < this.$slot.layer_reel_frame.children.length; index++) {
        this.$slot.layer_reel_frame.children.at(index).z = index * 1000;
    }
    this.initReels();
    this.makeUiScroll();
    this.$hitboxForNormal = this.create2DArray(this._reelWidth, this._reelWindowHeight, null);
    this.$fixedSymbolForBonus = this.create1DArray(this._bonusReelWidth, null);
    this.$fixedSymbolForPyramid = this.create2DArray(this._reelWidth, this._reelWindowHeight, null);
    this.$fixedSymbolForOriental = this.create2DArray(this._reelWidth, this._reelWindowHeight, null);
    this.$fixedSymbolForAtlantis = this.create2DArray(this.atlantisReelWidth, 1, null);
    this.$fixedSymbolForAtlantis[0] = this.create1DArray(9, null);
    for (let index = 1; index < this.atlantisReelWidth; index++) {
        this.$fixedSymbolForAtlantis[index][0] = null;
    }
    this.$ui_multipot_text[0] = this.$slot.reel_bg.num_mini.$text;
    this.$ui_multipot_text[1] = this.$slot.reel_bg.num_minor.$text;
    this.$ui_multipot_text[2] = this.$slot.reel_bg.num_major.$text;
    this.$ui_multipot_text[3] = this.$slot.reel_bg.num_ultra.$text;
    this.$ui_multipot_text[4] = this.$slot.reel_bg.num_grand.$text;

    // test
    //  this._slotLoginInfo.slotInfo.slotSaveTB = "464000,464000,464000/29,29,29/30,24,30/11A5S8.0K,22A10S16K,2A5S8.0K,15A10S16K,5A50S80K,6A5S8K,"
    if (this._slotLoginInfo.slotInfo.slotSaveTB && this._slotLoginInfo.slotInfo.slotSaveTB != 'no slotsave data') {
        this.initSlotSaveData(this._slotLoginInfo.slotInfo.slotSaveTB);
    }
    co(function * () {
        try {
            yield wait(2000);
            if (this._is_destroy) {
                return;
            }
            if (!this.isRecovery) {
                this._playScatterHitAnimationForNormal("stay", true);
                this.getFeaturedInfo();
            }
        }
        catch (e) {
            DUG.log(e);
        }
    }.bind(this));
}

/**
 * @override
 * 슬롯 입장, 보너스 진입시에 필요한 가상 릴셋을 제공한다.
 */
makeFakeReelSet(reelIndex)
{
    let index = CMiscUtils.getRandomIntInclusive(0, 51 - 1);
    if (this._slotLoginInfo.slotState != null) {
        // 슬롯 진입시 이전 스핀 결과가 있으면 가장 최근 스핀 결과를 설정한다.
        index = this._slotLoginInfo.slotState.reelBeforePredictionArray[reelIndex];
    }
    let predicArray = [];
    let spinType = (this.hasSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN)) ? 1 : 0;
    let reelArray = this._getReelArray(spinType);
    let starIndex = reelIndex * 51;  // 릴셋 길이로 변경 필요
    let limitIndex = starIndex + 51;
    for (let i = 0; i < this._reelWindowHeight; i++) {
        let symbolIndex = (starIndex + index + i);
        if (symbolIndex >= limitIndex) {
            symbolIndex = symbolIndex - 51;
        }
        predicArray.push(reelArray[symbolIndex]);
    }
    return [predicArray];
}

makeFakeReelSetBonus()
{
    let predicArray = [];
    for (let i = 0; i < this._reelWindowHeight; i++) {
        predicArray.push(CMiscUtils.getRandomIntInclusive(10004, 10011));
    }
    return [predicArray];
}

/**
 * onSpin
 */
onSpin()
{
    this._skip = false;
    this._bonusScatterAppearCount = 0;
    this._pickScatterAppearCount = 0;
    this.scatterAppearForAtlantis = false;
    this.$IdleProcessQueue.killAndDone();
    this.$hidePaylineProcessQueue.killAndDone();
    clearTimeout(this.countDownTimeout);
    clearTimeout(this.countDownTimeoutSound);

    // 모든 아이콘 알파값 복원
    this._setResultIconsAlpha(1);
    if (this.removeHitBoxFlag) {
        this.removeHitBoxForNormal();
        this.removeHitBoxFlag = false;
    }
    // 응답 지연 처리 이벤트 등록
    this.eventListener.on(this, 'responseDelayStart', this._onResponseDelayStartedBindMe ? this._onResponseDelayStartedBindMe : this._onResponseDelayStartedBindMe = this._onResponseDelayStarted.bind(this));

    if (this.isPick && !this.isSuperFreespin) {
        this.removeBottomUIEffect();
        this.pickCount--;
        this.$uiKeyText.text = `$ { this.pickCount }
        `;
        this.$uiKeyText.setAutoAlign();
        if (this.pickCount == 0) {
            this.pickEnd = true;
        }
        this.onFakeSpin();
    }
    else {
        if (this.isSuperFreespin) {
            this.setAvgTotalBet();
            this.superFreespinRemainCount--;
            this.setFreespinLeftcount(this.superFreespinRemainCount, this.superFreespinTotalCount);
            if (this.superFreespinRemainCount == 0) {
                this.superFreespinEnd = true;
                if (this.pickCount > 0) {
                    this.pickStart = true;
                    this.pickRetrigger = true;
                }
            }
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            this.freespinRemainCount--;
            this.setFreespinLeftcount(this.freespinRemainCount, this.freespinTotalCount);
        }
        super.onSpin();
    }
}

onSpinStart()
{
    this._currentSpinIntervalTime = (this._bSpinFast == true) ? this._spinFastIntervalTime : this._spinIntervalTime;
    this._currentSpinBaseTime = (this._bSpinFast == true) ? this._spinFastBaseTime : this._spinBaseTime;
    let reelStopSpeed = (this._bSpinFast == true) ? CSlotReelWrapper.STOP_SPEED.FAST : CSlotReelWrapper.STOP_SPEED.NORMAL;
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        super.onSpinStart();
    }
    else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
            if (this.$fixedSymbolForBonus[reelIndex]) {
                continue;
            }
            let $reel = this._getTypeReel(reelIndex, this.spinType);
            $reel.$slotReelWrapper.spin();
            let _duration_base = this._currentSpinBaseTime;
            $reel._duration_gap = 0.05 * reelIndex;
            $reel.$slotReelWrapper.$timeline.to($reel.$slotReelWrapper, _duration_base + $reel._duration_gap, {
                onComplete : this.onCompleteSpinPhaseBindMe ? this.onCompleteSpinPhaseBindMe : (this.onCompleteSpinPhaseBindMe = this.onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ $reel, reelStopSpeed ]
            });
            $reel._spin_phase = "SPIN";
            this._spinActiveReel.push($reel);
        }
    }
    else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
            let $reel = this._getTypeReel(reelIndex, this.spinType);
            let count = 0;
            for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
                    count++;
                }
            }
            if (count == this._reelWindowHeight) {
                continue;
            }
            $reel.$slotReelWrapper.spin();
            let _duration_base = this._currentSpinBaseTime;
            $reel._duration_gap = this._currentSpinIntervalTime * reelIndex;
            $reel.$slotReelWrapper.$timeline.to($reel.$slotReelWrapper, _duration_base + $reel._duration_gap, {
                onComplete : this.onCompleteSpinPhaseBindMe ? this.onCompleteSpinPhaseBindMe : (this.onCompleteSpinPhaseBindMe = this.onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ $reel, reelStopSpeed ]
            });
            $reel._spin_phase = 'SPIN';
            this._spinActiveReel.push($reel);
        }
    }
    else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        for (let reelIndex = 0; reelIndex < this.atlantisReelWidth; reelIndex++) {
            if (reelIndex == 0) {
                continue;
            }
            if (this.$fixedSymbolForAtlantis[reelIndex][0]) {
                continue;
            }
            let $reel = this._getTypeReel(reelIndex, this.spinType);

            $reel.$slotReelWrapper.spin();

            let _duration_base = this._currentSpinBaseTime;
            $reel._duration_gap = 0.05 * reelIndex;
            $reel.$slotReelWrapper.$timeline.to($reel.$slotReelWrapper, _duration_base + $reel._duration_gap, {
                onComplete : this.onCompleteSpinPhaseBindMe ? this.onCompleteSpinPhaseBindMe : (this.onCompleteSpinPhaseBindMe = this.onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ $reel, reelStopSpeed ]
            });
            $reel._spin_phase = 'SPIN';
            this._spinActiveReel.push($reel);
        }
    }
    else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
            let $reel = this._getTypeReel(reelIndex, this.spinType);

            $reel.$slotReelWrapper.spin();

            let _duration_base = this._currentSpinBaseTime;
            $reel._duration_gap = this._currentSpinIntervalTime * reelIndex;
            $reel.$slotReelWrapper.$timeline.to($reel.$slotReelWrapper, _duration_base + $reel._duration_gap, {
                onComplete : this.onCompleteSpinPhaseBindMe ? this.onCompleteSpinPhaseBindMe : (this.onCompleteSpinPhaseBindMe = this.onCompleteSpinPhase.bind(this)),
                onCompleteParams : [ $reel, reelStopSpeed ]
            });
            $reel._spin_phase = 'SPIN';
            this._spinActiveReel.push($reel);
        }
    }
}

onSpinAfter()
{
    super.onSpinAfter();
    if (this._bSpinAuto == true) {
        this.stopEffectSound("spin_reel");
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        if (this._bSpinFast == = true) {
            this.playEffectSound("snd_reel_fast_1", "spin_reel");
        }
        else {
            if (this.spinNum > 3) {
                this.spinNum = 1;
            }
            this.playEffectSound("snd_reel_spin_" + this.spinNum, "spin_reel");
            this.spinNum++;
        }
    }
}

_endCurrentSpin()
{
    DUG.log('end of spin');
    // 튜토리얼 진행중인 경우 처리
    if (DUG.common.userInfo.is_tutorial_slot == = true) {
        this.tutorialAward += this._currentResult.spinResult.payout.totalAward;

        let isTutorialEndSpin = this._currentResult.spinResult.slotState.featuredMap.te || 0;
        if (isTutorialEndSpin == 1) {
            this.$spinProcessQueue.add(this.onExitToLobbyBindMe ? this.onExitToLobbyBindMe : (this.onExitToLobbyBindMe = this.onExitToLobby.bind(this)));
            this.$spinProcessQueue.next(500);
            this._bSpinAuto = false;
            return;
        }
        else {
            this._bSpinAuto = true;
        }
    }

    if (this.isPick == = true) {
        DUG.log('pick');
        this._processEndSpin();
    }
    else if (this.isNextSpinNoMoneyIn()) {
        // 프리스핀
        DUG.log('no moneyIn');
        this.$spinProcessQueue.add(this.onAutoNextSpinStartBindMe ? this.onAutoNextSpinStartBindMe : (this.onAutoNextSpinStartBindMe = this.onAutoNextSpinStart.bind(this)));
        this.$spinProcessQueue.next(500);
    }
    else if (this._bSpinAuto == = true) {
        DUG.log('auto spin');
        this.$spinProcessQueue.add(this.onAutoNextSpinStartBindMe ? this.onAutoNextSpinStartBindMe : (this.onAutoNextSpinStartBindMe = this.onAutoNextSpinStart.bind(this)));
        this.$spinProcessQueue.next(500);
    }
    else {
        DUG.log('init this._bSpin');
        this._processEndSpin();
    }
}

onEndCurrentSpin()
{
    super.onEndCurrentSpin();
    DUG.common.settings.testMode = 0;
    DUG.common.settings.testMode2 = 0;
    DUG.common.settings.testMode3 = 0;
    DUG.common.settings.resetSave = 0;
    this.isClick = false;
    if (this.isRecovery) {
        this.isRecovery = false;
    }
    //빅윈 후 종료버튼누를때 같이 클릭되는 현상 수정
    if (this.isPick) {
        co(function * () {
            try {
                yield wait(100);
                if (this._is_destroy) {
                    return;
                }
                for (let index = 0; index < this.$tresureBox.length; index++) {
                    if (this.$tresureBox[index]) {
                        this.$tresureBox[index].setClick(true);
                    };
                }
            }
            catch (e) {
                DUG.log(e);
            }
        }.bind(this));
    }
}

/**
 * complete lazy load's Event Callback
 * @override
 * 리소스 로드가 끝나지 않은채 _onCheckBonusModeSevralProc() 호출될 경우 BGM 이 정상적으로 재생되지 않아 리소스 로드 완료시 보너스 모드인 경우 바꿔 재생되도록 오버라이드
 */
onCompleteLazyLoadingResources(resources)
{
    super.onCompleteLazyLoadingResources(resources);
    co(function * () {
        try {
            yield wait(100);
            if (this._is_destroy) {
                return;
            }
            let delay = 100;
            if (this.hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN) == false &&
                this.hasNextSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN) == false) {
                this.playEffectSound('snd_welcome');
                delay = 3000;
            }
            yield wait(delay);
            if (this._is_destroy) {
                return;
            }
            if (this.hasNextSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN) == true) {
                this.playBGMSound('bgm_bonus');
            }
            else if (this.hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN) == true) {
                if (this.isPick) {
                    this.playBGMSound("bgm_pick_" + String.fromCharCode(this.pickType + 96));
                }
                if (this.isSuperFreespin) {
                    this.playBGMSound("bgm_super_" + String.fromCharCode(this.pickType + 96));
                }
            }
            else {
                this.playBGMSound('bgm_normal_reel');
            }
        }
        catch (e) {
            DUG.log(e);
        }
    }.bind(this));
}

/**
 * spin 동작의 End 체크 함수.
 * @param {Object} $reel - reel instance
 * @param {number} stopSpeed - speed
 * @protected
 */
onCompleteSpinPhase($reel, stopSpeed = CSlotReelWrapper.STOP_SPEED.NORMAL)
{
    if (this._bSpinResponse == = false) {
        if (this._spinResponseLazyCount == = 0) {
            this.emit('responseDelayStart', this);
        }
        this.emit('responseDelaying', this, this._spinResponseLazyCount);
        // 응답을 받지 못한 경우 lazycount 증가
        this._spinResponseLazyCount++;
    }
    else {
        // 정상적인 상황
        let _reelIndex = $reel.$slotReelWrapper.reelIndex;
        let _changeReelIndex = _reelIndex;
        let _playLines = [];
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            _playLines = this._currentResult.spinResult.reels[_changeReelIndex].playLines;
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            _changeReelIndex = Math.floor(_reelIndex / this._reelWindowHeight);
            let iconId = this._currentResult.spinResult.reels[_changeReelIndex].playLines[_reelIndex % 9];
            if (iconId == 0) {
                iconId = CMiscUtils.getRandomIntInclusive(10004, 10011);
            }
            _playLines.push(iconId);
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            _playLines = this._currentResult.spinResult.reels[_changeReelIndex].playLines;
            for (let index = 0; index < _playLines.length; index++) {
                if (_playLines[index] < 200) {
                    _playLines[index] = CMiscUtils.getRandomIntInclusive(10004, 10011);
                }
            }
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            if (_reelIndex == 0) {
                _playLines = this._currentResult.spinResult.reels[_changeReelIndex].playLines;
            }
            else {
                _changeReelIndex = Math.floor((_reelIndex - 1) / this._reelWindowHeight) + 1;
                let iconId = this._currentResult.spinResult.reels[_changeReelIndex].playLines[(_reelIndex - 1) % 9] if (iconId == 0)
                {
                    iconId = CMiscUtils.getRandomIntInclusive(10004, 10011);
                }
                else
                {
                    if (this.$fixedSymbolForAtlantis[_reelIndex][0]) {
                        iconId = CMiscUtils.getRandomIntInclusive(10004, 10011);
                    }
                    else {
                        iconId = 300;
                    }
                }
                _playLines.push(iconId);
            };
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            _playLines = this._currentResult.spinResult.reels[_changeReelIndex].playLines;
            let scatter = 0;
            let coin = 0;
            for (let index = 0; index < _playLines.length; index++) {
                if (_playLines[index] == 420) {
                    scatter = index;
                    break;
                }
                if (_playLines[index] >= 430) {
                    coin = index;
                }
            }
            if (coin < scatter) {
                this.hitReelForOriental.push(_changeReelIndex);
            }
        }
        this.onAnimateSpinStop($reel, _playLines, stopSpeed);
    }
}

/**
 * @override
 */
onAnimatedSpinStop($reel)
{
    let _delay = 0;

    if (this._currentResult && this._currentResult.spinResult) {
        for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
            let $reel = this._getTypeReel(reelIndex, this.spinType);
            let playlines = this._currentResult.spinResult.reels[reelIndex].playLines;
            let appearScatter = false for (let rowIndex = 0; rowIndex < playlines.length; rowIndex++)
            {
                if (this.isNormalScatter(playlines[rowIndex])) {
                    appearScatter = true;
                }
            }
            if (appearScatter) {
                $reel.z = reelIndex + 10;
            }
            else {
                $reel.z = reelIndex;
            }
        }
        this.$slot.layer_reel_frame.children.sort(this.depthCompare);
    }

    super.onAnimatedSpinStop($reel, _delay);
    if (this._spinActiveReel.length == 0) {
        this.stopEffectSound('spin_reel');
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        $reel.$slotReelWrapper.hideNeedlessIcon();
    }
}

/**
 * 스핀 응답 지연 시작
 */
_onResponseDelayStarted()
{
    this.eventListener.off(this, 'responseDelayStart', this._onResponseDelayStartedBindMe);

    this.eventListener.on(this, 'responseDelayEnd', this._onResponseDelayEndedBindMe ? this._onResponseDelayEndedBindMe : this._onResponseDelayEndedBindMe = this._onResponseDelayEnded.bind(this));

    // 스핀 응답 지연에 필요한 처리 기능 구현
}

/**
 * 스핀 응답 지연 종료
 */
_onResponseDelayEnded()
{
    this.eventListener.off(this, 'responseDelayEnd', this._onResponseDelayEndedBindMe);
}

/**
 * @override
 */
_initPayoutComponents(options)
{
    options = options || {};
    super._initPayoutComponents(options);
}

/**
 * @override
 * 페이아웃 전 필요한 처리
 */
onRespinseSpinResultBeforePayout()
{
}

/**
 * @override
 * 다음 스핀 요청에 대한 응답 처리
 */
onResponseSpinResult(response)
{
    super.onResponseSpinResult(response);

    this.eventListener.off(this, 'responseDelayStart', this._onResponseDelayStartedBindMe);

    if (this._currentResult.spinResult.payout.totalAward > 0) {
        // 페이아웃 연출 종료 시점에 획득금액 업데이트 요청
        this.$spinProcessQueue.insertBefore(this.onEndCurrentSpinBindMe, this.onUpdateUserInfoBindMe ? this.onUpdateUserInfoBindMe : (this.onUpdateUserInfoBindMe = this.onUpdateUserInfo.bind(this)));
    }
}

/**
 * @protected
 * _playHitAnimation
 */
_playHitAnimation()
{
    let _hitIcons = this._findAllHitIcons();

    if (_hitIcons.length < 1) {
        return;
    }
    this.removeHitBoxFlag = true;
    for (let reelIndex = 0; reelIndex < this.slotReels.length; reelIndex++) {
        let $slotReelWrapper = this.slotReels[reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.NORMAL);
        for (var rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
            let $icon = _resultIcons[rowIndex];
            if (ArrayUtils.contains(_hitIcons, $icon)) {
                this._playIconHitAnimation($icon);
                this.$hitboxForNormal[reelIndex][rowIndex] = this.$spineContainer.borrow("s346_ui_hit_box");
                this.$slot.layer_reel_frame.addChild(this.$hitboxForNormal[reelIndex][rowIndex]);
                this.$hitboxForNormal[reelIndex][rowIndex].setAnimation("hit", true);
                this.$hitboxForNormal[reelIndex][rowIndex].adjustPosition($reel, rowIndex, 1);
            }
            else {
                this._playIconNoHitAnimation($icon);
            }
        }
    }
}

removeHitBoxForNormal()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            if (this.$hitboxForNormal[reelIndex][rowIndex] && this.$hitboxForNormal[reelIndex][rowIndex].parent) {
                this.$hitboxForNormal[reelIndex][rowIndex].parent.removeChild(this.$hitboxForNormal[reelIndex][rowIndex]);
                this.$hitboxForNormal[reelIndex][rowIndex].destroy({children : true});
                this.$hitboxForNormal[reelIndex][rowIndex] = null;
            }
        }
    }
}

/**
 * _playScatterHitAnimation
 */
_playScatterHitAnimationForNormal(_action, _loop = false, scatter = 0)
{
    if (scatter == 1) {
        this.playEffectSound("snd_scatter_hit");
    }
    if (scatter == 2) {
        this.playEffectSound("snd_scatter_hit_2");
    }
    for (var reelIndex = 0; reelIndex < this.slotReels.length; reelIndex++) {
        var $slotReelWrapper = this.slotReels[reelIndex];
        var _resultIcons = $slotReelWrapper.getResultIcons();
        for (var rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
            var $icon = _resultIcons[rowIndex];
            let _iconId = $icon._dug_icon_id;
            if (_action == "hit") {
                this._playIconNoHitAnimation($icon);
            }
            if (!this.isNormalScatter(_iconId)) {
                continue;
            }
            if (scatter == 1 && _iconId >= 30) {
                continue;
            }
            if (scatter == 2 && _iconId < 30) {
                continue;
            }
            this._playIconNoHitAnimation($icon, 1)
                let _aniName = "hit";
            let target = this.allReelFrame[this.spinType].animation;
            if (rowIndex != 0 && rowIndex != 8) {
                target = this.$slot.layer_reel_up;
            }
            let $aIcon = this._playIconAnimation({
                icon : $icon,
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
onTransferControl()
{
    this._topBarControl = false;
    if (this.isSpinOnTransferControl()) {
        if (!this.isPick) {
            this.onSpin();
        }
        else {
            if (DUG.common.settings.SLOT_HAS_TOPBAR) {
                this._isAvailableClickTopbar = false;
                this.$topBar.checkSpinStatusUI({isRunSpin : true});
            }
            this.$spinProcessQueue.add(this.onWaitPick.bind(this));
            this.$spinProcessQueue.next();
        }
    }
    else {
        DUG.common.log.warn('[onTransferControl] It is not possible to spin.');
    }
}

_playIconAnimation(options)
{
    let $icon = options.icon,
        $animationTarget = options.animationTarget || this.allReelFrame[this.spinType].animation,
        _action = options.action,
        _animationClassType = options.animationClassType,
        _reelIndex = options.reelIndex,
        _rowIndex = options.rowIndex,
        _size = options.size || 1,
        _iconVisible = options.iconVisible == = true,
        _autoRelease = options.autoRelease == = true;

    options.animationTarget = $animationTarget;

    // if (!$icon) {
    // 	$icon = this.slotReels[_reelIndex].getResultIcon(_rowIndex);
    // 	options.icon = $icon;
    // }

    let $reel = this._getTypeReel($icon._reelIndex, this.spinType),
        $aIcon = null;

    _reelIndex = $icon._reelIndex;
    _rowIndex = $icon._rowIndex;
    if (_animationClassType) {
        // 아이콘 컴포넌트 위치에 다른 아이콘을 넣을 경우
        // 기존 아이콘 숨기고
        $icon.visible = _iconVisible;
        $aIcon = this.$iconContainer.borrowByClassType(_animationClassType);
        $aIcon.gotoAndPlay(_action);
        $icon._dug_animation_icon = $aIcon;

        this._animateAnimationOhterIcons.push($icon);
    }
    else if ($icon.timeline && $icon.timeline._labels[_action] != = undefined) {
        // 히트 연출을 포함하는 심볼이면
        $icon.gotoAndPlay(_action);
        $icon._dug_previous_parent = $icon.parent;
        $icon._dug_previous_x = $icon.x;
        $icon._dug_previous_y = $icon.y;
        $aIcon = $icon;

        this._animateAnimationIcons.push($icon);
    }
    else if ($icon.playAnimaiton) {
        $aIcon = $icon.playAnimaiton(options);
        this._animationIcons.push($icon);
    }

    if ($aIcon == = null) {
        // 히트 연출을 포함하지 않는 심볼이면 컨테이너에서 빌려옴
        // 기존 아이콘 숨기고
        $icon.visible = _iconVisible;
        // 히트 연출 아이콘 추가
        $aIcon = this.$animationIconContainer.borrow($icon._dug_icon_id, _action);
        $aIcon.loop = options.loop == = true;
        $aIcon.gotoAndPlay(0);
        if (_autoRelease == = false) {
            $icon._dug_animation_icon = $aIcon;
        }
        $aIcon.onComplete = function()
        {
            DUG.log('onComplete() : ' + $aIcon._dug_icon_id);
            if (_autoRelease == = true) {
                $aIcon.stop();
                $aIcon.parent.removeChild($aIcon);
                this.$animationIconContainer.release($aIcon);
                $icon.visible = true;
            }
        }
        .bind(this);
        this._spritesheetAnimationIcons.push($icon);
    }
    _rowIndex = this.checkTopBottomForLongSymbol($icon, _rowIndex);
    if ($aIcon.adjustPosition) {
        $aIcon.adjustPosition($reel, _rowIndex, _size);
    }
    else {
        $aIcon.x = $reel.x + (this._iconWidth - $aIcon.width) / 2;
        $aIcon.y = $reel.y + (this._iconHeight * _rowIndex) + ((this._iconHeight * _size) - $aIcon.height) / 2;
    }
    $animationTarget.addChild($aIcon);
    $aIcon._reelIndex = _reelIndex;
    $aIcon._rowIndex = _rowIndex;
    return $aIcon;
}

checkTopBottomForLongSymbol($icon, _rowIndex)
{
    if (this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        return _rowIndex;
    }
    let _iconId = $icon._dug_icon_id;
    if (_rowIndex == 0 && (_iconId == 22 || _iconId == 32)) {
        _rowIndex -= 1;
    }
    if (_rowIndex == 8 && (_iconId == 20 || _iconId == 30)) {
        _rowIndex += 1;
    }
    return _rowIndex;
}

onIdleSymbol()
{
    this.$spinProcessQueue.next(1);

    if (this.isSuperFreespin || this.isPick || this.isFreespin) return;
    if (this._bSpinAuto == true) return;
    if (this._currentResult == undefined) return;
    if (this._currentResult.spinResult.payout.totalAward == 0 && this._currentResult.spinResult.payout.isJackpot == false) {
        if (this.$IdleProcessQueue) {
            this.$IdleProcessQueue.killAndDone();
            this.$IdleProcessQueue.add(
                this.onIdleSymbolIconEffectBindMe
                    ? this.onIdleSymbolIconEffectBindMe
                    : (this.onIdleSymbolIconEffectBindMe =
                           this.onIdleSymbolIconEffect.bind(this)));
            this.$IdleProcessQueue.next(700);
        }
        else {
            this.onIdleSymbolIconEffect();
        }
    }
}

onIdleSymbolIconEffect()
{
    this.$IdleProcessQueue.killAndDone();
    this._initAllAnimations();
    this._playScatterHitAnimationForNormal("stay", true);
}

isNormalScatter(_iconId)
{
    return _iconId == 20 || _iconId == 21 || _iconId == 22 || _iconId == 30 || _iconId == 31 || _iconId == 32;
}
isNormalFreespinScatter(_iconId)
{
    return _iconId == 20 || _iconId == 21 || _iconId == 22;
}

initReels()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
        let _reelArray = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 203, 210 ];

        $reel.$slotReelWrapper = this._createSlotReelWrapperForPyramid($reel, {
            slotConfig : this._slotConfig,
            iconContainer : this.$iconContainer,
            reelIndex : reelIndex,
            reelArray : _reelArray,
            reelWindowHeight : this._reelWindowHeight,
            iconHeight : this._iconHeight,
            startPosition : 0,
            findIndexFunc : this.findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : this._iconLayerOrder,
            fakeReelSet : this.makeFakeReelSetBonus()
        });
        this.slotReelsForPyramid[reelIndex] = $reel.$slotReelWrapper;
        this.slotReelsForPyramid[reelIndex].visible = false;
    }

    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);
        let _reelArray = [ 420, 430, 430, 430, 430, 430, 430, 430, 430, 440 ];
        $reel.$slotReelWrapper = this._createSlotReelWrapperForOriental($reel, {
            slotConfig : this._slotConfig,
            iconContainer : this.$iconContainer,
            reelIndex : reelIndex,
            reelArray : _reelArray,
            reelWindowHeight : this._reelWindowHeight,
            iconHeight : this._iconHeight,
            startPosition : 0,
            findIndexFunc : this.findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : this._iconLayerOrder,
            fakeReelSet : [ 430, 430, 430, 430, 430, 430, 430, 430, 430 ]
        });
        this.slotReelsForOriental[reelIndex] = $reel.$slotReelWrapper;
        this.slotReelsForOriental[reelIndex].visible = false;
    }

    this._bonusReelWidth = this._reelWidth * this._reelWindowHeight;
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        let _reelArray = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 50, 51, 52, 53, 54, 55, 60 ];
        let _reelArrayTop = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 40, 41, 42, 45 ];
        $reel.$slotReelWrapper = this._createSlotReelWrapperForBonus($reel, {
            slotConfig : this._slotConfig,
            iconContainer : this.$iconContainer,
            reelIndex : reelIndex,
            reelArray : reelIndex % 9 == 0 ? _reelArrayTop : _reelArray,
            reelWindowHeight : 1,
            iconHeight : this._iconHeight,
            startPosition : 0,
            findIndexFunc : this.findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : this._iconLayerOrder,
            fakeReelSet : [10005]
        });
        this.slotReelsForBonus[reelIndex] = $reel.$slotReelWrapper;
        if (reelIndex % 9 != 0) {
            $reel.$slotReelWrapper.y += 6;
        }
        $reel.$slotReelWrapper.hideNeedlessIcon();
    }

    this.atlantisReelWidth = this._reelWindowHeight * 3 + 1;
    for (let reelIndex = 0; reelIndex < this.atlantisReelWidth; reelIndex++) {
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
        let _reelArrayFirst = [ 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000 ];
        let _reelArray = [ 10004, 10005, 10006, 10007, 10008, 10009, 10010, 10011, 300 ];
        $reel.$slotReelWrapper = this._createSlotReelWrapperForAtlantis($reel, {
            slotConfig : this._slotConfig,
            iconContainer : this.$iconContainer,
            reelIndex : reelIndex,
            reelArray : reelIndex == 0 ? _reelArrayFirst : _reelArray,
            reelWindowHeight : reelIndex == 0 ? this._reelWindowHeight : 1,
            iconHeight : this._iconHeight,
            startPosition : 0,
            findIndexFunc : this.findIndexFunc,  //필요한 경우에만 각 슬롯별로 구현. 디폴트는 CSlotReelWrapper.js의 findIndex()
            iconLayerOrder : this._iconLayerOrder,
            fakeReelSet : reelIndex == 0 ? this.makeFakeReelSetBonus() : [10005]
        });
        this.slotReelsForAtlantis[reelIndex] = $reel.$slotReelWrapper;
        this.slotReelsForAtlantis[reelIndex].visible = false;
    }
    for (let index = 1; index < this.allReelFrame.length; index++) {
        this.allReelFrame[index].visible = false;
    }
}

/**
 * 해당 인덱스 릴 인스턴스 반환
 * @protected
 *
 * @param {number} reelIndex - 릴 인덱스 (0 부터 시작)
 * @returns {Object} 릴 객체
 */
_getTypeReel(reelIndex, type)
{
    if (type == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        return this.$slot.layer_reel_frame['reel_' + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        return this.$slot.layer_reel_frame_bonus['reel_' + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        return this.$slot.layer_reel_frame_pyramid['reel_' + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        return this.$slot.layer_reel_frame_atlantis['reel_' + (reelIndex + 1)];
    }
    else if (type == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        return this.$slot.layer_reel_frame_oriental['reel_' + (reelIndex + 1)];
    }
}

setMultiPotText()
{
    for (let index = 0; index < this.$ui_multipot_text.length; index++) {
        let fixPay = this._totalBet * this.multipot[index + 1] / 10;
        if (this.isSuperFreespin) {
            fixPay = this._avgTotalBet * this.multipot[index + 1] / 10;
        }
        this.$ui_multipot_text[index].text = CMiscUtils.moneyFormatter(fixPay);
        this.$ui_multipot_text[index].setAutoAlign();
    }
}

setReelVisible(type)
{
    for (let index = 0; index < this.allReelFrame.length; index++) {
        if (index == type) {
            this.allReelFrame[index].visible = true;
        }
        else {
            this.allReelFrame[index].visible = false;
        }
    }
}

/**
 * 페이아웃 증가 Update에 대한 Callback
 *  @protected
 *  @param {number} value - update 되는 값.
 */
onPayoutUpdateValue(value)
{
    // 페이아웃 스킵 확인
    if (this._bUsePayoutSkip == = false) {
        super.onPayoutUpdateValue(value);
    }
    else {
        if (this._bPayoutSkipping == = true) {
            if (this.$timeline_payout) {
                this.$timeline_payout.kill();
                this.$timeline_payout.clear();
            }
            this.onPayoutCompleteValue(value, value.options.onComplete, value.options.nextQueueDelay);
        }
        else {
            if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
                super.onPayoutUpdateValue(value);
            }
            // 보너스 팟 공통화
            if (this._bUse_update_bonus_pot == true) {
                let _totalAward = value.init + value.value * (value.result - value.init);
                this._incremental_bonus_pot = this._incremental_bonus_pot_end + _totalAward;
                this.bonuspot_number = this._incremental_bonus_pot;
            }
        }
    }
}

/**
 * @protected
 * 결과 아이콘들의 알파값을 설정
 */
_setResultIconsAlpha(alpha)
{
    let width = this.slotReels.length;
    for (let reelIndex = 0; reelIndex < width; reelIndex++) {
        let $slotReelWrapper = this.slotReels[reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        let _beforeIcons = $slotReelWrapper._beforeIcons;
        for (let rowIndex = 0, length = _resultIcons.length; rowIndex < length; rowIndex++) {
            let $icon = _resultIcons[rowIndex];
            $icon.visible = true;
            $icon.alpha = alpha;
        }
        for (let rowIndex = 0, length = _beforeIcons.length; rowIndex < length; rowIndex++) {
            let $icon = _beforeIcons[rowIndex];
            $icon.visible = true;
            $icon.alpha = alpha;
        }
    }
}

checkReelDelay(responseResult)
{
    if (this.firstSpinForSuper == = true) {
        this.getFeaturedInfo();
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            this.superFreespinStartAnimationForPyramid();
            for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
                let delay = reelIndex > 0 ? 4000 : 2000;
                this.setReelDelayTime(reelIndex, delay);
            }
            this._bSpinReelDelaying = true;
        }
        this.firstSpinForSuper = false;
    }
}

onBigwinBefore()
{
    this.isClick = true;
    this.removeEndPopup();
    if (this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS && !this.isPick) {
        this.playEffectSpine("s346_ui_bigwin", this.$slot.popup_container, "hit");
        this.playEffectSound("snd_omen_bigwin");
    }
    else {
        this.pausedBGMSound(true);
    }

    co(function * () {
        try {
            this.$spinProcessQueue.next(1000);
        }
        catch (error) {
            this.errorLog(error);
            this.$spinProcessQueue.next();
        }
    }.bind(this));
}

onBigwin(options)
{
    if (this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS && !this.isPick) {
        this.pausedBGMSound(true);
    }
    super.onBigwin(options);
}

onBigwinEnd()
{
    if ((this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS || this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) && this.pickCount == 0) {
        this.setNormal();
        this.$bottomUI.setSpinAgain();
        this.superTotalAward = 0;
    }
    else {
        this.pausedBGMSound(false);
    }
    // bgm 복구
    if (this.hasSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN)) {
        this.bonuspot_number = this._currentResult.spinResult.freespin.totalAward;

        // 보너스 팟 공통화 빅윈 end 시 보너스팟 갱신.
        if (this._bUse_update_bonus_pot == true) {
            this._incremental_bonus_pot = this._incremental_bonus_pot_end = this._currentResult.spinResult.freespin.totalAward;
        }
    }
    // 빅윈 엔드가 호출되면 오토스핀일때, 빅윈프로퍼티를 가져와서 딜레이 타임을 넣어준다.
    this.$spinProcessQueue.next(this._delayAfterBigwin());
}

playBlackLayer(action, parent)
{
    this.removeBlackLayer();
    this.$blackLayer = this.$spineContainer.borrow("s346_ui_black_layer");
    parent.addChild(this.$blackLayer);
    this.$blackLayer.setAnimation(action);
}

removeBlackLayer(action = false)
{
    if (action) {
        this.$blackLayer.setAnimation("out");
        this.$blackLayer.addAnimationComplete(
            "out", (entry) = > {
                co(function * () {
                    try {
                        yield wait(0);
                        if (this._is_destroy) {
                            return;
                        }
                        this.removeBlackLayer(false);
                    }
                    catch (error) {
                        DUG.log(error);
                    }
                }.bind(this));
            });
    }
    else {
        if (this.$blackLayer) {
            this.$blackLayer.parent.removeChild(this.$blackLayer);
            this.$blackLayer.destroy({children : true});
            this.$blackLayer = null;
        }
    }
}

removeInfoMap()
{
    this.removeTresureBox();
    this.removeUiTresureMap();
    this.removeInfoPig(false, false);
    this.removeExitButton();
    this.removeChoiceMap();
    this.$uiScroll.buttonMode = true;
    this.$uiScroll.interactive = true;
    this.isClick = false;
    this.openMap = false;
}

/**
 * spin Click 처리
 *  @protected
 *  @returns {boolean} status
 */
onClickSpin(e)
{
    if (this.isPick) {
        return false;
    }
    if (this.openMap) {
        this.removeInfoMap();
    }
    super.onClickSpin();
}

onClickBetUp(e)
{
    if (this.isPick) {
        return false;
    }
    if (this.openMap) {
        this.removeInfoMap();
    }
    super.onClickBetUp(e);
}

onClickBetDown(e)
{
    if (this.isPick) {
        return false;
    }
    if (this.openMap) {
        this.removeInfoMap();
    }
    super.onClickBetDown();
}

/**
 * maxbet 버튼 클릭 처리
 * @override
 */
onClickMaxBet(e)
{
    if (this.isPick) {
        return false;
    }
    if (this.openMap) {
        this.removeInfoMap();
    }
    super.onClickMaxBet();
}

/**
 *  @protected
 * pay table 버튼 클릭 처리
 */
onClickPaytable()
{
    if (this.isPick) {
        return false;
    }
    if (this.openMap) {
        this.removeInfoMap();
    }
    super.onClickPaytable(e);
}

/**
 *  오토스핀 클릭이 가능한 상태인지 체크
 * @override
 * @returns {boolean} status
 */
_isEnableAuto()
{
    if (this.isPick) {
        return false;
    }
    if (this.openMap) {
        this.removeInfoMap();
    }
    return true;
}

checkBetButtonStatusByBetLevel()
{
    if (this.isPick || this.isSuperFreespin) {
        this.$bottomUI.setDisableBetBtn();
    }
    else {
        super.checkBetButtonStatusByBetLevel();
    }
}
// _isEnablePayoutSkip() {
// 	if(this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL){
// 		return false;
// 	}
// 	super._isEnablePayoutSkip();
// }
// DUC-Take5 통합 추가
clickSpinSkipProgress()
{
    // 릴 스킵
    if (this._isEnablePayoutSkip()) {
        if (this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            return;
        }
        this.onPayoutSkipProgress();
    }
    else if (this._isEnableSkipSpin()) {
        this.onSpinSkip();
    }
}
/**
 * 터치 영역 생성
 * @protected
 */
_createInteractiveRegion(x, y, w, h, color = 0x000000, alpha = 0.0)
{
    let rv = new PIXI.Graphics();
    rv.lineStyle(1, color, 1);
    rv.beginFill(color, 1);
    rv.drawRect(-(w / 2), -(h / 2), w, h);
    rv.endFill();
    rv.alpha = alpha;
    rv.x = x;
    rv.y = y;

    rv.hitArea = new PIXI.Rectangle(-(w / 2), -(h / 2), w, h);

    return rv;
}

setResultIconVisible()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        for (let rowIndex = 0, length = _resultIcons.length; rowIndex < length; rowIndex++) {
            _resultIcons[rowIndex].visible = true;
        }
    }
}

playMultipotPopup(pay, isSuper)
{
    this.removeMultipotPopup();
    let index = isSuper ? this.getMultipotForSuper(pay) : pay;
    this.$multipotPopup = this.$spineContainer.borrow("s346_ui_multipot_popup");
    this.$slot.popup_container.addChild(this.$multipotPopup);
    let _text = this.$multipotPopup.addBitmapText("num", {size : 74, font : "s346_num_popup_multipot", align : "center", alignY : "center", alignY : "center"}, this.eventListener);
    _text.text = this.$ui_multipot_text[index - 1].text;
    _text.setAutoAlign();
    co(function * () {
        try {
            yield wait(500);
            if (this._is_destroy) {
                return;
            }
            this.playEffectSound("snd_" + this.multipots[index]);
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this, index));
    this.$multipotPopup.setAnimation(this.multipots[index]);
    this.$multipotPopup.addAnimationComplete(
        this.multipots[index], (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.removeMultipotPopup();
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}
////////////////////////////////////////////////////////////////////////////////////////////
//슬롯 세이브
////////////////////////////////////////////////////////////////////////////////////////////
/**
 * customFreespinEnd Callback
 * @override
 */
onCustomFreespinCallback()
{
    super.onCustomFreespinCallback();
    this.requestGetSlotSaveString();
}

/**
 * tbl_user_slot_save 의 값 조회 리시브 성공
 * 개별 슬롯에서 오버라이드 해서 데이터를 파싱 하도록 해야함.
 */
onReceiveSuccessSlotSaveString(response)
{
    this.initSlotSaveData(response.data);
}

initSlotSaveData(slotSaveData)
{
    this.pickResultInfo = [ {}, {}, {}, {} ];
    let strVal = "";
    if (slotSaveData) {
        strVal = slotSaveData.toString();
    }
    if (strVal != "" && strVal != "no slotsave data") {
        let splitTemp = strVal.split('/');
        if (splitTemp.length > 3) {
            let info = splitTemp[3].split(",");
            info.forEach(string = > {
                if (string != "") {
                    if (string.indexOf("P") >= 0) {
                        let index = string.split("P");
                        this.pickResultInfo["1"][index[0]] = index[1];
                    }
                    else if (string.indexOf("A") >= 0) {
                        let index = string.split("A");
                        this.pickResultInfo["2"][index[0]] = index[1];
                    }
                    else if (string.indexOf("O") >= 0) {
                        let index = string.split("O");
                        this.pickResultInfo["3"][index[0]] = index[1];
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
_createSlotReelWrapperForBonus(instance, options)
{
    options.onBounceStopSpin = this._onBounceStopSpinForBonus.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    let randomStart = CMiscUtils.getRandomIntInclusive(0, options.reelArray.length - 1);
    options.startPosition = randomStart;
    options.spineContainer = this.$spineContainer;
    options.findIndexFunc = this.findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureBonusReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
_onBounceStopSpinForBonus($slotReelWrapper)
{
    const reelIndex = $slotReelWrapper.reelIndex;
    // 릴 스핀 종료 재생
    let _resultIcons = $slotReelWrapper.getResultIcons();
    let boomAppear = false;
    let coinAppear = false;
    let boxAppear = false;
    let extrAppear = false;
    for (let rowIndex = 0; rowIndex < 1; rowIndex++) {
        let $icon = _resultIcons[rowIndex];
        let _iconId = $icon._dug_icon_id;
        if (!this.isBonusScatter(_iconId)) {
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
        let _aniName = "hit";
        if (_iconId == 40 || _iconId == 41 || _iconId == 42 || _iconId == 60) {
            _aniName = "appear";
        }
        let $aIcon = this._playIconAnimation({
            icon : $icon,
            action : "appear",
            loop : false,
            aniName : _aniName,
            animationTarget : this.$slot.layer_reel_up,
            reelIndex : reelIndex,
            rowIndex : rowIndex
        });
        this.setIconTextForBonus($aIcon, _iconId);
        if ($aIcon.$attach) {
            $aIcon.$attach.setAnimation("appear");
        }
        if ((_iconId >= 50 && _iconId <= 55) || _iconId == 60) {
            if (this.$openBox[reelIndex]) {
                this.$openBox[reelIndex].visible = true;
                if (this.openState[reelIndex] == true) {
                    this.$openBox[reelIndex].setAnimation("hold");
                    this.$openBox[reelIndex].open = true;
                    this.setTintForBonus($icon, false);
                    this.setTintForBonus($aIcon, false);
                }
                else {
                    this.$openBox[reelIndex].setAnimation("off");
                    this.setTintForBonus($icon, true);
                    this.setTintForBonus($aIcon, true);
                }
            }
            if (this.$openBoxFrame[reelIndex] && this.$openBoxFrame[reelIndex].parent) {
                this.$openBoxFrame[reelIndex].parent.removeChild(this.$openBoxFrame[reelIndex]);
                this.$openBoxFrame[reelIndex].destroy({children : true});
                this.$openBoxFrame[reelIndex] = null;
            }
        }
    }
    this.playEffectSound("snd_reel_stop_2");
    if (coinAppear && !boxAppear && !boomAppear && !extrAppear) {
        this.playEffectSound("snd_coin_appear");
    }
    if (boxAppear && !boomAppear && !extrAppear) {
        this.playEffectSound("snd_box_appear");
    }
    if (boomAppear && !extrAppear) {
        this.playEffectSound("snd_bomb_appear");
    }
    if (extrAppear) {
        this.playEffectSound("snd_extra_appear");
    }
}

freespinProc()
{
    this._initAllAnimations();
    this.setOpenState();
    //고정 심볼 생성
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (!this.$fixedSymbolForBonus[reelIndex]) {
            let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
            let $slotReelWrapper = this.slotReelsForBonus[reelIndex];
            let $icon = $slotReelWrapper.getResultIcon(0);
            let _iconId = $icon._dug_icon_id;
            if ((_iconId >= 50 && _iconId <= 55) || (_iconId == 60)) {
                $icon.visible = false;
                let spineName = "";
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
                this.$fixedSymbolForBonus[reelIndex] = this.$spineContainer.borrow(spineName);
                this.$fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
                this.$fixedSymbolForBonus[reelIndex]._rowIndex = 0;
                this.$fixedSymbolForBonus[reelIndex]._dug_icon_id = _iconId;
                this.$slot.layer_reel_up.addChild(this.$fixedSymbolForBonus[reelIndex]);
                this.$fixedSymbolForBonus[reelIndex].adjustPosition($reel, 0, 1);
                this.setIconTextForBonus(this.$fixedSymbolForBonus[reelIndex], this.$fixedSymbolForBonus[reelIndex]._dug_icon_id);
                if (this.$openBox[reelIndex].open == false) {
                    this.setTintForBonus(this.$fixedSymbolForBonus[reelIndex], true);
                }
                else {
                    this.setTintForBonus(this.$fixedSymbolForBonus[reelIndex], false);
                }
            }
        }
    }
    this.$spinProcessQueue.next(100);
}

freespinProc1()
{
    // topreel stay 연출
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        let $slotReelWrapper = this.slotReelsForBonus[reelIndex];
        let $icon = $slotReelWrapper.getResultIcon(0);
        let _iconId = $icon._dug_icon_id;
        if (_iconId >= 40 && _iconId <= 42) {
            let index = 0;
            for (let i = reelIndex + 1; i < reelIndex + 9; i++) {
                if (this.$openBox[i].open == false) {
                    index = i;
                    break;
                };
            }
            if (index == 0) {
                continue;
            }
            let $aIcon = this._playIconAnimation({
                icon : $icon,
                action : "hit",
                loop : true,
                aniName : "stay",
                animationTarget : this.$slot.layer_reel_up_3,
                reelIndex : reelIndex,
            });
        }
    }
    // topreel hit연출
    co(function * () {
        try {
            for (let reelWidth = 0; reelWidth < this._reelWidth; reelWidth++) {
                let _reelIndex = 9 * reelWidth;
                let _reelMax = 9 * (reelWidth + 1);
                for (let reelIndex = _reelIndex; reelIndex < _reelMax; reelIndex++) {
                    if (this._is_destroy) {
                        return;
                    }
                    let $slotReelWrapper = this.slotReelsForBonus[reelIndex];
                    let $icon = $slotReelWrapper.getResultIcon(0);
                    let _iconId = $icon._dug_icon_id;
                    if (_iconId >= 40 && _iconId <= 42) {
                        let index = 0;
                        for (let i = _reelIndex + 1; i < _reelMax; i++) {
                            if (this.$openBox[i].open == false) {
                                index = i;
                                break;
                            };
                        }
                        if (index == 0) {
                            break;
                        }
                        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
                        let spine = this.$spineContainer.borrow("s346_ui_focus_box");
                        this.$slot.layer_reel_up.addChild(spine);
                        spine.setAnimation("hit");
                        spine.adjustPosition($reel, 0, 1);
                        spine.addAnimationComplete(
                            "hit", (entry) = > {
                                co(function * () {
                                    try {
                                        yield wait(0);
                                        if (this._is_destroy) {
                                            return;
                                        }
                                        spine.parent.removeChild(spine);
                                        spine = null;
                                    }
                                    catch (error) {
                                        DUG.log(error);
                                    }
                                }.bind(this));
                            });
                        this.playEffectSound("snd_bomb_hit");
                        this.playEffectSound("snd_bomb_hit_" + (_iconId % 40 + 1));
                        this._playIconAnimation({
                            icon : $icon,
                            action : "hit",
                            animationTarget : this.$slot.layer_reel_up,
                            aniName : "down_" + (index % 9),
                            reelIndex : reelIndex,
                        });
                        yield wait(833);
                        if (this._is_destroy) {
                            return;
                        }
                        this.removeBox(reelWidth);
                        yield wait(300);
                        if (this._is_destroy) {
                            return;
                        }
                    }
                    else if (_iconId == 45) {
                        this.playEffectSound("snd_retrigger");
                        this.playEffectSpine("s346_ui_spin_add", this.$slot.eff_container, "hit");
                        let $aIcon = this._playIconAnimation({
                            icon : $icon,
                            action : "hit",
                            animationTarget : this.$slot.layer_reel_up,
                            reelIndex : reelIndex,
                        });
                        yield wait(1666);
                        if (this._is_destroy) {
                            return;
                        }
                        this.freespinRemainCount++;
                        this.freespinTotalCount++;
                        this.setFreespinLeftcount(this.freespinRemainCount, this.freespinTotalCount);
                        this._writeSlotActionLog({
                            category_info : '',
                            action : 'BONUS_RETRIGGER',
                            action_info : "Total Count: " + this.freespinTotalCount,
                            isplaynow : 1
                        });
                    }
                }
            }
            if (this._is_destroy) {
                return;
            }
            this.setOpenState();
            this.updateStaySymbolForBonus();
            this.$spinProcessQueue.next(500);
        }
        catch (e) {
            DUG.log(e);
        }
    }.bind(this));
}

freespinPayoutProc()
{
    co(function * () {
        try {
            let index = 0;
            for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
                if (this._is_destroy) {
                    return;
                }
                if (this.$fixedSymbolForBonus[reelIndex]) {
                    let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
                    let _iconId = this.$fixedSymbolForBonus[reelIndex]._dug_icon_id;
                    if (_iconId == 60 && this.$openBox[reelIndex].open == true) {
                        let _delay = 1000;
                        if (this.$fixedSymbolForBonus[reelIndex] && this.$fixedSymbolForBonus[reelIndex].parent) {
                            this.$fixedSymbolForBonus[reelIndex].parent.removeChild(this.$fixedSymbolForBonus[reelIndex]);
                            this.$fixedSymbolForBonus[reelIndex].destroy({children : true});
                            this.$fixedSymbolForBonus[reelIndex] = null;
                        }
                        //연출이 겹치는 부분이 있어 새로 생성하여 연출
                        this.playEffectSound("snd_box_open");
                        let spine = this.$spineContainer.borrow("s346_icon_mystery_box");
                        this.$slot.layer_reel_up.addChild(spine);
                        spine.adjustPosition($reel, 0, 1);
                        spine.setAnimation("hit");
                        spine.addAnimationComplete(
                            "hit", (entry) = > {
                                co(function * () {
                                    try {
                                        yield wait(0);
                                        if (this._is_destroy) {
                                            return;
                                        }
                                        spine.parent.removeChild(spine);
                                    }
                                    catch (error) {
                                        DUG.log(error);
                                    }
                                }.bind(this));
                            });
                        yield wait(1000);
                        if (this._is_destroy) {
                            return;
                        }
                        let changeId = this.boxResultForFreespin[index];
                        index++;
                        let spineName = "s346_icon_dia_hit";
                        if (changeId == 50) {
                            spineName = "s346_icon_coin_hit";
                        }
                        if (changeId == 31 || changeId == 32) {
                            spineName = "s346_icon_key";
                        }
                        if (changeId > 50) {
                            this.fixedPayoutForBonus[reelIndex] = this.multipot[changeId % 50];
                        }
                        this.$fixedSymbolForBonus[reelIndex] = this.$spineContainer.borrow(spineName);
                        this.$fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
                        this.$fixedSymbolForBonus[reelIndex]._dug_icon_id = changeId;
                        this.$slot.layer_reel_up.addChild(this.$fixedSymbolForBonus[reelIndex]);
                        this.$fixedSymbolForBonus[reelIndex].adjustPosition($reel, 0, 1);
                        this.setIconTextForBonus(this.$fixedSymbolForBonus[reelIndex], changeId);
                        let action = "box_open";
                        if (changeId == 31) {
                            action = "appear_2";
                        }
                        if (changeId == 32) {
                            action = "appear_3";
                        }
                        this.$fixedSymbolForBonus[reelIndex].setAnimation(action);
                        if (this.$fixedSymbolForBonus[reelIndex].$attach) {
                            this.$fixedSymbolForBonus[reelIndex].$attach.setAnimation("box_open");
                        }
                        if (changeId == 31 || changeId == 32) {
                            _delay = 2000;
                        }
                        yield wait(_delay);
                        if (this._is_destroy) {
                            return;
                        }
                    }
                }
            }
            if (this._is_destroy) {
                return;
            }
            this.updateStaySymbolForBonus();
            this.$spinProcessQueue.next(500);
        }
        catch (e) {
            DUG.log(e);
        }
    }.bind(this));
}

freespinPayoutProc1()
{
    if (this.$bottomUI) {
        this.removeBottomUIEffect();
    }
    // this._initAllAnimations();
    this.setBoxDepth();
    this.playBlackLayer("bonus", this.$slot.black);
    let slotState = this.getSlotState() || {};
    let totalPayout = slotState.freespinTotalAward;
    this.setOnlyBonusPotPay(true);
    this.payoutType = "1";
    this.playEffectSoundWithOption("snd_payout_" + this.payoutType, "snd_payout_" + this.payoutType, true);
    this.changeBGMVolume(0.5);
    co(function * () {
        try {
            let totalPay = 0;
            for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
                if (this.$fixedSymbolForBonus[reelIndex]) {
                    let $icon = this.$fixedSymbolForBonus[reelIndex];
                    let _iconId = $icon._dug_icon_id;
                    if (_iconId >= 50 && this.$openBox[reelIndex].open == true) {
                        let _delay = 0;
                        let _action = "hit";
                        if (_iconId > 50) {
                            this.playEffectSound("snd_text_count");
                            _action = "focus";
                            _delay = 2000;
                        }
                        else {
                            this.playEffectSound("snd_coin_count");
                            _delay = 300;
                        }
                        let spine = this.$spineContainer.borrow("s346_ui_hit_box");
                        this.$slot.layer_reel_up.addChild(spine);
                        spine.setAnimation("hit");
                        spine.x = $icon.x;
                        spine.y = $icon.y;
                        spine.addAnimationComplete(
                            "hit", (entry) = > {
                                co(function * () {
                                    try {
                                        yield wait(0);
                                        if (this._is_destroy) {
                                            return;
                                        }
                                        if (spine && spine.parent) {
                                            spine.parent.removeChild(spine);
                                            spine = null;
                                        }
                                    }
                                    catch (error) {
                                        DUG.log(error);
                                    }
                                }.bind(this));
                            });
                        $icon.setAnimation(_action);
                        if ($icon.$attach) {
                            $icon.$attach.setAnimation(_action)
                        }
                        if (_iconId > 50) {
                            let index = _iconId % 10;
                            if (index > 0) {
                                yield wait(1000);
                                if (this._is_destroy) {
                                    return;
                                }
                                this.playEffectSound("snd_text_popup_a");
                                this.playMultipotPopup(index, false);
                            }
                        }
                        totalPay += this.fixedPayoutForBonus[reelIndex] * this._totalBet / 10;
                        this.bonuspot_number = totalPay;
                        if (this.$bottomUI) {
                            this.$bottomUI.playPayoutEffect();
                        }
                        yield wait(_delay);
                        if (this._is_destroy) {
                            return;
                        }
                    }
                }
            }
            this.stopEffectSound("snd_payout_" + this.payoutType);
            this.playEffectSound("snd_payout_" + this.payoutType + "_end");
            this._incremental_bonus_pot_end = 0;
            this._incremental_bonus_pot = totalPayout;
            this.bonuspot_number = totalPayout;
            this.changeBGMVolume(1, 1);
            this.$spinProcessQueue.next(2000);
        }
        catch (e) {
            DUG.log(e);
        }
    }.bind(this, totalPayout));
}

freespinPayoutProc2()
{
    this.removeBlackLayer();
    this.removeMultipotPopup();
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (this.$fixedSymbolForBonus[reelIndex]) {
            let $icon = this.$fixedSymbolForBonus[reelIndex];
            let _iconId = $icon._dug_icon_id;
            if (_iconId >= 50) {
                $icon.setAnimation("on");
                if ($icon.$attach) {
                    $icon.$attach.setAnimation("on")
                }
            }
        }
    }
    this.$spinProcessQueue.next(0);
}

onStartBonusGames()
{
    this.freespinRemainCount = 5;
    this.freespinTotalCount = 5;
    this._writeSlotActionLog({
        category_info : '',
        action : 'BONUS_ENTER: ' + this.freespinTotalCount,
        action_info : '',
        isplaynow : 1
    });
    this._initAllAnimations();
    this._changeTotalBet();
    this.stopBGMSound();
    this.setBottomUI(false);
    this._playScatterHitAnimationForNormal("hit", false, 1);
    this.spinType = CSlot346PiggyAdventure.SPIN_TYPE.BONUS;
    this.$spinProcessQueue.next(2000);
}

onStartBonusGames1()
{
    this.playBGMSound("bgm_bonus");
    this.playEffectSound("snd_bonus_opening_1");
    this.playEffectSpine("s346_ui_bonus", this.$slot.popup_container, "hit");
    this.$spinProcessQueue.next(3000);
}

onStartBonusGames2()
{
    this._initAllAnimations();
    this.playSlotStayForBonus();
    this.setFreespinLeftcount(this.freespinRemainCount, this.freespinTotalCount);
    this.setSlotMode("bonus");
    this._incremental_bonus_pot = 0;
    this._incremental_bonus_pot_end = 0;
    this.bonuspot_number = 0;
    this.setOnlyBonusPotPay(false);
    this.$bottomUI.setGoodLuck();
    this.fixBoxSymbol();
    this.openState = this.create1DArray(this._bonusReelWidth, false);
    this.setOpenState();
    this.$spinProcessQueue.next(2000);
}

onEndBonusGames()
{
    this.$spinProcessQueue.next(100);
}

onEndBonusGames1()
{
    this._writeSlotActionLog({
        category_info : '',
        action : 'BONUS_EXIT',
        action_info : '',
        isplaynow : 1
    });

    this.stopBGMSound();
    this.playEffectSound("snd_bonus_ending_1");
    this.removeEndPopup();
    this.$endPopup = this.$spineContainer.borrow("s346_ui_closing");
    this.$endPopup.setAnimation("hit");
    this.$slot.popup_container.addChild(this.$endPopup);
    let _text = this.$endPopup.addBitmapText("num", {size : 74, font : "s346_num_congratulations", align : "center", alignY : "center"}, this.eventListener);
    _text.text = CMiscUtils.moneyFormatter(this.getSlotState().freespinTotalAward);
    _text.setAutoAlign();
    this.$endPopup.addAnimationComplete(
        "hit", (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.removeEndPopup();
                    this.$spinProcessQueue.next(0);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
    this.playEffectSpine("s346_ui_payout", this.$slot.popup_container, "hit");
}

onEndBonusGames2()
{
    if (this.pickType > 0 && this.pickCount > 0) {
        this.$spinProcessQueue.next(0);
    }
    else {
        this.setNormal();
        this.$bottomUI.setSpinAgain();
        this.$spinProcessQueue.next(100);
    }
}

removeMultipotPopup()
{
    if (this.$multipotPopup && this.$multipotPopup.parent) {
        this.$multipotPopup.parent.removeChild(this.$multipotPopup);
        this.$multipotPopup.destroy({children : true});
        this.$multipotPopup = null;
    }
}

removeEndPopup()
{
    if (this.$endPopup && this.$endPopup.parent) {
        this.$endPopup.parent.removeChild(this.$endPopup);
        this.$endPopup.destroy({children : true});
        this.$endPopup = null;
    }
}

removeOpenPopup()
{
    if (this.$openPopup && this.$openPopup.parent) {
        this.$openPopup.parent.removeChild(this.$openPopup);
        this.$openPopup.destroy({children : true});
        this.$openPopup = null;
    }
}

removeFixedSymbolForBonus()
{
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (this.$fixedSymbolForBonus[reelIndex] && this.$fixedSymbolForBonus[reelIndex].parent) {
            this.$fixedSymbolForBonus[reelIndex].parent.removeChild(this.$fixedSymbolForBonus[reelIndex]);
            this.$fixedSymbolForBonus[reelIndex].destroy({children : true});
            this.$fixedSymbolForBonus[reelIndex] = null;
        }
    }
}

removeOpenBox()
{
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (this.$openBox[reelIndex] && this.$openBox[reelIndex].parent) {
            this.$openBox[reelIndex].parent.removeChild(this.$openBox[reelIndex]);
            this.$openBox[reelIndex].destroy({children : true});
            this.$openBox[reelIndex] = null;
        }
    }
}

removeOpenFrame()
{
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (this.$openBoxFrame[reelIndex] && this.$openBoxFrame[reelIndex].parent) {
            this.$openBoxFrame[reelIndex].parent.removeChild(this.$openBoxFrame[reelIndex]);
            this.$openBoxFrame[reelIndex].destroy({children : true});
            this.$openBoxFrame[reelIndex] = null;
        }
    }
}

calcCoinIncreaseTimeForfreespin()
{
    let _duration = 0;
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (this.$fixedSymbolForBonus[reelIndex]) {
            let $icon = this.$fixedSymbolForBonus[reelIndex];
            let _iconId = $icon._dug_icon_id;
            if (_iconId >= 50 && this.$openBox[reelIndex].open == true) {
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
setOpenState()
{
    for (let i = 0; i < this._reelWidth; i++) {
        for (let j = 0; j < this.digHeightInfo[i]; j++) {
            this.openState[i * 9 + j + 1] = true;
        }
    }
}

setBoxDepth()
{
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (this.$fixedSymbolForBonus[reelIndex]) {
            let $icon = this.$fixedSymbolForBonus[reelIndex];
            let _iconId = $icon._dug_icon_id;
            if (this.$fixedSymbolForBonus[reelIndex]) {
                if (_iconId >= 50 && this.$openBox[reelIndex].open == true) {
                    this.$slot.layer_reel_up.addChild(this.$openBox[reelIndex]);
                    this.$slot.layer_reel_up_2.addChild(this.$fixedSymbolForBonus[reelIndex]);
                }
                if (this.$openBox[reelIndex].open == false) {
                    this.$slot.layer_reel_up_3.addChild(this.$fixedSymbolForBonus[reelIndex]);
                }
            }
        }
    }
}

recoveryProcForBonus()
{
    this.openState = this.create1DArray(this._bonusReelWidth, false);
    this.setOpenState();
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        let _iconId = this.fixedPlaylineForBonus[reelIndex];
        if ((_iconId >= 50 && _iconId <= 55) || (_iconId == 60)) {
            let spineName = "";
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
            this.$fixedSymbolForBonus[reelIndex] = this.$spineContainer.borrow(spineName);
            this.$fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
            this.$fixedSymbolForBonus[reelIndex]._rowIndex = 0;
            this.$fixedSymbolForBonus[reelIndex]._dug_icon_id = _iconId;
        }
    }
    this.$openBox = this.create1DArray(this._bonusReelWidth, null);
    this.$openBoxFrame = this.create1DArray(this._bonusReelWidth, null);
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        let $slotReelWrapper = this.slotReelsForBonus[reelIndex];
        $slotReelWrapper.replaceResultIconById(0, CMiscUtils.getRandomIntInclusive(10004, 10011));
        if (this.$fixedSymbolForBonus[reelIndex]) {
            $slotReelWrapper.getResultIcon(0).visible = false;
        }
        if (reelIndex % 9 > 0) {
            this.$openBox[reelIndex] = this.$spineContainer.borrow("s346_icon_block");
            if (this.openState)
                this.$openBox[reelIndex].setAnimation("off");
            this.$slot.layer_reel_down.addChild(this.$openBox[reelIndex]);
            this.$openBox[reelIndex].adjustPosition($reel, 0, 1);
            this.$openBox[reelIndex].open = false;
            if (reelIndex % 9 < 3) {
                this.$openBox[reelIndex].visible = false;
                this.$openBox[reelIndex].open = true;
            }
            else {
                this.$openBoxFrame[reelIndex] = this.$spineContainer.borrow("s346_icon_block");
                this.$openBoxFrame[reelIndex].setAnimation("off_frame");
                this.$slot.layer_reel_up_3.addChild(this.$openBoxFrame[reelIndex]);
                this.$openBoxFrame[reelIndex].adjustPosition($reel, 0, 1);
            }
        }
    }
    for (let reelWidth = 0; reelWidth < this._reelWidth; reelWidth++) {
        this.removeBox(reelWidth);
    }
    this.updateStaySymbolForBonus();
}
/*
 * 탐험 보너스 시작 시 히트 박스 및 고정 심볼 생성
 */
fixBoxSymbol()
{
    this.$openBox = this.create1DArray(this._bonusReelWidth, null);
    this.$openBoxFrame = this.create1DArray(this._bonusReelWidth, null);
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
        let $slotReelWrapper = this.slotReelsForBonus[reelIndex];
        $slotReelWrapper.replaceResultIconById(0, CMiscUtils.getRandomIntInclusive(10004, 10011));
        if (reelIndex % 9 > 0) {
            this.$openBox[reelIndex] = this.$spineContainer.borrow("s346_icon_block");
            this.$openBox[reelIndex].setAnimation("off");
            this.$slot.layer_reel_down.addChild(this.$openBox[reelIndex]);
            this.$openBox[reelIndex].adjustPosition($reel, 0, 1);
            this.$openBox[reelIndex].open = false;
            if (reelIndex % 9 < 3) {
                this.$openBox[reelIndex].visible = false;
                this.$openBox[reelIndex].open = true;
            }
            else {
                this.$openBoxFrame[reelIndex] = this.$spineContainer.borrow("s346_icon_block");
                this.$openBoxFrame[reelIndex].setAnimation("off_frame");
                this.$slot.layer_reel_up_3.addChild(this.$openBoxFrame[reelIndex]);
                this.$openBoxFrame[reelIndex].adjustPosition($reel, 0, 1);
            }
        }
        if (reelIndex % 9 <= 2) {
            $slotReelWrapper.setOpen(true);
        }
        else {
            $slotReelWrapper.setOpen(false);
        }
        if (reelIndex % 9 == 8) {
            $slotReelWrapper.replaceResultIconById(0, 60);
            $slotReelWrapper.getResultIcon(0).visible = false;
            this.$fixedSymbolForBonus[reelIndex] = this.$spineContainer.borrow("s346_icon_mystery_box");
            this.$fixedSymbolForBonus[reelIndex].setAnimation("in");
            this.$fixedSymbolForBonus[reelIndex].addAnimationComplete(
                "in", (entry) = > {
                    co(function * () {
                        try {
                            yield wait(0);
                            if (this._is_destroy) {
                                return;
                            }
                            this.setTintForBonus(this.$fixedSymbolForBonus[reelIndex], true);
                        }
                        catch (error) {
                            DUG.log(error);
                        }
                    }.bind(this));
                });
            this.$fixedSymbolForBonus[reelIndex].addAnimation("on");
            this.$slot.layer_reel_up.addChild(this.$fixedSymbolForBonus[reelIndex]);
            this.$fixedSymbolForBonus[reelIndex]._reelIndex = reelIndex;
            this.$fixedSymbolForBonus[reelIndex]._dug_icon_id = 60;
            this.$fixedSymbolForBonus[reelIndex].adjustPosition($reel, 0, 1);
            this.$openBox[reelIndex].setAnimation("off");
            if (this.$openBoxFrame[reelIndex] && this.$openBoxFrame[reelIndex].parent) {
                this.$openBoxFrame[reelIndex].parent.removeChild(this.$openBoxFrame[reelIndex]);
                this.$openBoxFrame[reelIndex].destroy({children : true});
                this.$openBoxFrame[reelIndex] = null;
            }
        }
    }
}

/*
 * 탐험보너스 stay 갱신
 */
updateStaySymbolForBonus()
{
    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
        if (this.$fixedSymbolForBonus[reelIndex]) {
            let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.BONUS);
            let $icon = this.$fixedSymbolForBonus[reelIndex];
            let _iconId = $icon._dug_icon_id;
            let _action = "stay";
            if (_iconId == 31) {
                _action = "loop_2";
            }
            else if (_iconId == 32) {
                _action = "loop_3";
            }
            this.$slot.layer_reel_up.addChild($icon);
            $icon.adjustPosition($reel, 0, 1);
            this.setIconTextForBonus($icon, $icon._dug_icon_id);
            if (this.$openBox[reelIndex].open == false) {
                _action = "on";
                this.setTintForBonus($icon, true);
            }
            else {
                this.setTintForBonus($icon, false);
            }
            $icon.setAnimation(_action, true);
        }
    }
}

setTintForBonus($icon, tint)
{
    let _tint = 0x8c8c8c;
    if (!tint) {
        _tint = 0xFFFFFF;
    }
    $icon.tint = _tint;
    if ($icon.$attach) {
        $icon.$attach.tint = _tint;
    }
    let slot = $icon.getSlotContainer("num");
    if (slot && slot.$text) {
        slot.$text.tint = _tint;
    }
}

/*
 * 상태에 따라 탐험보너스 히트박스 지워줌
 */
removeBox(reelWidth)
{
    let _reelIndex = 9 * reelWidth + 1;
    let _reelMax = 9 * (reelWidth + 1);
    for (let reelIndex = _reelIndex; reelIndex < _reelMax; reelIndex++) {
        if (this.openState[reelIndex] == true) {
            let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
            if (this.$fixedSymbolForBonus[reelIndex]) {
                this.$openBox[reelIndex].visible = true;
                this.$openBox[reelIndex].setAnimation("hold");
                this.setTintForBonus(this.$fixedSymbolForBonus[reelIndex], false);
            }
            else {
                this.$openBox[reelIndex].visible = false;
            }
            this.$openBox[reelIndex].open = true;
            $slotReelWrapper.setOpen(true);

            if (this.$openBoxFrame[reelIndex] && this.$openBoxFrame[reelIndex].parent) {
                this.$openBoxFrame[reelIndex].parent.removeChild(this.$openBoxFrame[reelIndex]);
                this.$openBoxFrame[reelIndex].destroy({children : true});
                this.$openBoxFrame[reelIndex] = null;
            }
        }
    }
}

playSlotStayForBonus()
{
    this.$staySpine = this.$spineContainer.borrow("s346_ui_reel_change");
    this.$slot.eff_container.addChild(this.$staySpine);
    if (this.isRecovery) {
        this.$staySpine.setAnimation("loop", true);
    }
    else {
        this.$staySpine.setAnimation("hit");
        this.$staySpine.addAnimation("loop", true);
    }
}

isBonusScatter(_iconId)
{
    return _iconId == 40 || _iconId == 41 || _iconId == 42 || _iconId == 45 ||
           _iconId == 50 || _iconId == 51 || _iconId == 52 || _iconId == 53 || _iconId == 54 || _iconId == 55 ||
           _iconId == 60;
}

/*
 * 탐험 보너스 아이콘 텍스트
 */
setIconTextForBonus(_icon, _iconId, random = false)
{
    if (_icon == undefined || (!(_iconId >= 50 && _iconId <= 55))) {
        return;
    }
    if (_icon.$attach && _icon.$attach.parent) {
        _icon.$attach.parent.removeChild(_icon.$attach);
        _icon.$attach.destroy({children : true});
        _icon.$attach = null;
    }
    _iconId = _iconId % 50;
    let reelIndex = _icon._reelIndex;
    let fixPay = 0;
    if (random) {
        let pay = [ 30, 25, 20, 15, 10, 5, 4, 3, 2, 1 ];
        fixPay = this._totalBet * pay[CMiscUtils.getRandomIntInclusive(0, pay.length - 1)] / 10;
    }
    else {
        fixPay = this.fixedPayoutForBonus[reelIndex] * this._totalBet / 10;
    }
    if (_iconId == 0) {
        let _text = _icon.addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
        _text.text = CMiscUtils.MakeUnitString(fixPay);
        _text.setAutoAlign();
    }
    else {
        let spineName = "s346_icon_" + this.multipots[_iconId];
        let _multipot = this.$spineContainer.borrow(spineName);
        _multipot.setAnimation("on");
        _icon.addChild(_multipot);
        _icon.$attach = _multipot;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 픽 보너스 함수
////////////////////////////////////////////////////////////////////////////////////////////
onStartPickBonusGames()
{
    this.pickStart = false;
    this.setBottomUI(false);
    this._changeTotalBet();
    if (this.pickRetrigger) {
        this._writeSlotActionLog({
            category_info : '',
            action : 'PICK RETRIGGER: ' + this.pickCount,
            action_info : this.pickType,
            isplaynow : 1
        });
        this.$spinProcessQueue.next(0);
    }
    else {
        this._writeSlotActionLog({
            category_info : '',
            action : 'PICK ENTER: ' + this.pickCount,
            action_info : this.pickType,
            isplaynow : 1
        });
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            this.makeUiKey();
        }
        this.$spinProcessQueue.next(500);
    }
}

onStartPickBonusGames1()
{
    if (this.pickRetrigger) {
        this.$spinProcessQueue.next(0);
    }
    else {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            this._initAllAnimations();
            this.stopBGMSound();
            this.removeUiScroll();
            this.makeUiKey();
            this._playScatterHitAnimationForNormal("hit", false, 2);
            co(function * () {
                try {
                    yield wait(2000);
                    if (this._is_destroy) {
                        return;
                    }
                    this.$uiKey.setAnimation("save");
                    this.$uiKeyText.text = `$ { this.pickCount }
                    `;
                    this.$uiKeyText.setAutoAlign();
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
            this.$spinProcessQueue.next(3333);
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            co(function * () {
                try {
                    let count = 0;
                    for (let reelIndex = 0; reelIndex < this._bonusReelWidth; reelIndex++) {
                        if (this.$fixedSymbolForBonus[reelIndex]) {
                            let $icon = this.$fixedSymbolForBonus[reelIndex];
                            let iconId = this.$fixedSymbolForBonus[reelIndex]._dug_icon_id;
                            if (iconId == 31 || iconId == 32) {
                                let _action = "save_2" if (iconId == 32)
                                {
                                    _action = "save_3";
                                }
                                this.playEffectSound("snd_key_collect");
                                this.$slot.layer_reel_up_2.addChild($icon);
                                this.$slot.black.addChild(this.$uiKey);
                                $icon.setAnimation(_action);
                                $icon.addAnimationComplete(
                                    _action, (entry) = > {
                                        co(function * () {
                                            try {
                                                yield wait(0);
                                                if (this._is_destroy) {
                                                    return;
                                                }
                                                this.$fixedSymbolForBonus[reelIndex].visible = false;
                                            }
                                            catch (error) {
                                                DUG.log(error);
                                            }
                                        }.bind(this));
                                    });
                                // reelup, eff 좌표차이
                                TweenMax.to($icon, 1.0,
                                            {
                                                ease : Power0.easeOut,
                                                x : (693 - 415) + 280.5,
                                                y : (760 - 338 - 67) - 415
                                            });
                                yield wait(1000);
                                if (this._is_destroy) {
                                    return;
                                }
                                this.$uiKey.setAnimation("save");
                                count += (iconId % 10) + 1 this.$uiKeyText.text = `$ { count }
                                `;
                                this.$uiKeyText.setAutoAlign();
                                //열쇠 적립창으로 날리기
                            }
                        }
                    }
                    this.$spinProcessQueue.next(1000);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        }
    }
}

onStartPickBonusGames2()
{
    //맵선택
    this.$slot.eff_container.addChild(this.$uiKey);
    this.isPick = true;
    if (this.pickRetrigger) {
        let _type = String.fromCharCode(this.pickType + 96);
        this.changeBG(true, _type, true);
    }
    else {
        this.makeChoiceMap();
    }
}

onStartPickBonusGames3()
{
    this.isClick = true;
    this.pickRetrigger = false;
    this.$spinProcessQueue.next(0);
}

onEndPickBonusGames()
{
    this.removeInfoPig(true, true);
    this.changeBG(false, "normal", true);
}

onEndPickBonusGames1()
{
    this.isPick = false;
    this.pickEnd = false;
    this.setBottomUI(true);
    this.$bottomUI.setSpinAgain();
    this.$spinProcessQueue.next(0);
}

pickProc()
{
    let isSuper = false;
    let isBigwin = false;
    let delay = 333;
    let _featuredMap = this.getFeaturedMap();
    if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT) {
        if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT > 0) {
            isSuper = true;
            this.$tresureBox[this.userPick].setSuper();
        }
    }
    if (!isSuper) {
        for (let index = 0; index < this.$tresureBox.length; index++) {
            if (this.$tresureBox[index] && this.pickType > 0) {
                this.$tresureBox[index].setPay(this.pickResultInfo[`$ { this.pickType }`][`$ { index }`]);
                this.$tresureBox[index].setClick(false);
            };
        }
        if (this.pickResultInfo[`$ { this.pickType }`][`$ { this.userPick }`]) {
            if (this.pickResultInfo[`$ { this.pickType }`][`$ { this.userPick }`].indexOf("100S") >= 0) {
                isBigwin = true;
            }
            if (this.pickResultInfo[`$ { this.pickType }`][`$ { this.userPick }`].indexOf("50S") >= 0) {
                isBigwin = true;
            }
        }
    }
    this.isClick = true;
    this.stopEffectSound("snd_pick_count");
    this.$tresureBox[this.userPick].open();
    ArrayUtils.removeAt(this.remainPickInfo, this.userPick);
    if (isSuper) {
        this.playEffectSound("snd_pick_super");
        this.stopBGMSound();
        this.pickResultInfo[`$ { this.pickType }`] = {};
        this.spinType = this.pickType + 1;
        delay = 4000;
    }
    if (isBigwin) {
        this.playEffectSound("snd_pick_bigwin");
        delay = 1166;
    }
    if (isSuper || isBigwin) {
        let action = isSuper ? "super" : "bigwin";
        if (this.$infoPig) {
            this.$infoPig.setAnimation(action);
            if (isSuper) {
                this.$infoPig.addAnimationComplete(
                    action, (entry) = > {
                        co(function * () {
                            try {
                                yield wait(0);
                                if (this._is_destroy) {
                                    return;
                                }
                                this.removeInfoPig(true, true);
                            }
                            catch (error) {
                                DUG.log(error);
                            }
                        }.bind(this));
                    });
            }
            else {
                this.$infoPig.addAnimation("loop", true);
            }
        }
    }
    else {
        this.playEffectSound("snd_pick");
    }
    this.userPick = -1;
    this.$spinProcessQueue.next(delay);
}

calcNewCoinIncreaseTimeForPick(targetAward)
{
    // 기본 0.5초
    let retVal = 0.5;
    let _award = targetAward || (this._currentResult && this._currentResult.spinResult.payout.totalAward) || 0;
    let _multi = Math.floor(_award / this._avgTotalBet);

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

onWaitPick()
{
    if (!this.isPick) {
        return;
    }
    if (DUG.common.settings.SLOT_HAS_TOPBAR) {
        this._isAvailableClickTopbar = false;
        this.$topBar.checkSpinStatusUI({isRunSpin : true});
    }
    // TODO: 카운트 다운을 넣는다
    this.countDownTimeout = setTimeout(this.playCountDown.bind(this), 5000);
    this.countDownTimeoutSound = setTimeout(this.playCountDownSound.bind(this), 5000);
    this.$spinProcessQueue.next(0);
}

makeUiKey()
{
    this.removeKey();
    this.$uiKey = this.$spineContainer.borrow("s346_ui_key_info");
    this.$slot.eff_container.addChild(this.$uiKey);
    this.$uiKey.setAnimation("appear");
    this.$uiKeyText = this.$uiKey.addBitmapText("num", {size : 46, font : "s346_num_key_collect", align : "center", alignY : "center", alignY : "center"}, this.eventListener);
    if (this.isRecovery) {
        this.$uiKeyText.text = `$ { this.pickCount }
        `;
        this.$uiKeyText.setAutoAlign();
    }
}

playCountDownSound()
{
    clearTimeout(this.countDownTimeoutSound);
    this.playEffectSoundWithOption("snd_pick_count", "snd_pick_count");
    this.countDownTimeoutSound = setTimeout(this.playCountDownSound.bind(this), 1000);
}

playCountDown()
{
    clearTimeout(this.countDownTimeout);
    this.removeCountDown();
    this.$countDown = this.$spineContainer.borrow('s346_ui_count_down');
    this.$slot.popup_container.addChild(this.$countDown);
    this.$countDown.setAnimation('hit');
    this.$countDown.addAnimationComplete(
        'hit', (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.stopEffectSound("snd_pick_count");
                    this.removeCountDown();
                    this.userPick = this.remainPickInfo[CMiscUtils.getRandomIntInclusive(0, this.remainPickInfo.length - 1)];
                    ArrayUtils.removeAt(this.remainPickInfo, this.userPick);
                    this.isClick = true;
                    if (this.$tresureBox[this.userPick]) {
                        this.$tresureBox[this.userPick].setOpen(true);
                    }
                    this.onSpin();
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}

makeUiScroll()
{
    this.removeUiScroll();
    this.$uiScroll = this.$spineContainer.borrow("s346_ui_scroll");
    this.$slot.eff_container.addChild(this.$uiScroll);
    this.$uiScroll.setAnimation("stay", true);
    let position = this.getSlotLocalPosision(this.$uiScroll, "scroll_all");
    this._makeButtonForScroll(this.$uiScroll);
}

/*
 * 지도 선택
 */
makeChoiceMap()
{
    this.removeChoiceMap();
    this.$uiChoiceMap = this.$spineContainer.borrow("s346_ui_map");
    this.$slot.eff_container.addChild(this.$uiChoiceMap);
    if (this.isPick) {
        this.setBonustime(true);
        this.setAvgTotalBet();
        this.setOnlyBonusPotPay(false);
        this.$bottomUI.setGoodLuck();
        this.stopBGMSound() this.playBGMSound("bgm_pick_compass");
        this.playEffectSound("snd_intro_compass");
        co(function * () {
            try {
                yield wait(2500);
                if (this._is_destroy) {
                    return;
                }
                this.playEffectSound("snd_compass_appear");
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this));
        this.$uiChoiceMap.setAnimation("bonus_in");
        this.$uiChoiceMap.addAnimation("loop", true);
        this.$compass = this.$spineContainer.borrow("s346_ui_compass");
        this.$slot.eff_container.addChild(this.$compass);
        this.$compass.setAnimation("in");
        this.$compass.addAnimationComplete(
            "in", (entry) = > {
                co(function * () {
                    try {
                        yield wait(0);
                        if (this._is_destroy) {
                            return;
                        }
                        this.removeUiScroll();
                        this.playCompassAnimation();
                    }
                    catch (error) {
                        DUG.log(error);
                    }
                }.bind(this));
            });
    }
    else {
        this.makeInfoPig(false);
        this.$uiChoiceMap.setAnimation("in");
        this.$uiChoiceMap.addAnimationComplete(
            "in", (entry) = > {
                co(function * () {
                    try {
                        yield wait(0);
                        if (this._is_destroy) {
                            return;
                        }
                        this.$uiScroll.buttonMode = false;
                        this.$uiScroll.interactive = false;
                        this.isClick = false;
                        this.$exitButton = this.$spineContainer.borrow("s346_ui_out_button");
                        this.$slot.popup_container.addChild(this.$exitButton);
                        this.$exitButton.setAnimation("on");
                        let position = this.getSlotLocalPosision(this.$uiChoiceMap, "num_4");
                        this._makeButtonForChoiceMapExit(this.$exitButton);
                    }
                    catch (error) {
                        DUG.log(error);
                    }
                }.bind(this));
            });
        let _slot = [ "num_1", "num_2", "num_3", "num_5", "num_6", "num_7" ];
        for (let index = 0; index < 3; index++) {
            let slot = this.$uiChoiceMap.getSlotContainer(_slot[index]);
            let $button = this._createInteractiveRegion(0, 0, 300, 160, 0xFFFFFF, 0);
            // a,b,c
            let type = String.fromCharCode(index + 97);
            if ($button) {
                this._makeButtonForChoiceMap($button, type);
                slot.addChild($button);
            }
        }
        for (let index = 0; index < 3; index++) {
            let slot = this.$uiChoiceMap.getSlotContainer(_slot[index + 3]);
            this.$paytalbeInfo[index] = this.$spineContainer.borrow("s346_ui_bonus_map_info");
            this.$paytalbeInfo[index].setAnimation("out", true);
            slot.addChild(this.$paytalbeInfo[index]);
            this.$paytalbeInfo[index].scale.y = -1;
            this._makeButtonForPaytalble(this.$paytalbeInfo[index], index);
        }
    }
}

makeInfoPig(isTresureMap)
{
    this.removeInfoPig();
    this.$infoPig = this.$spineContainer.borrow("s346_ui_indi_pig");
    this.$slot.eff_container2.addChild(this.$infoPig);
    if (isTresureMap) {
        if (this.pickRetrigger || this.isRecovery) {
            this.$infoPig.setAnimation("loop", true);
        }
        else {
            this.$infoPig.setAnimation("bonus_in");
            this.$infoPig.addAnimation("loop", true);
            this.playEffectSound("snd_intro_pick");
        }
    }
    else {
        this.playEffectSound("snd_map");
        this.$infoPig.setAnimation("in");
        this.$infoPig.addAnimation("in_loop", true);
    }
}

changeBG(isTresureMap, type, out = false)
{
    this.isClick = true;
    this.playChangeBG(out ? "out" : "hit", type, isTresureMap);
    let _type = "normal";
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
    co(function * () {
        try {
            yield wait(1000);
            if (this._is_destroy) {
                return;
            }
            this.setChangeBG(isTresureMap, type, _type);
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this, isTresureMap, type, _type));
}

playChangeBG(_action, type, isTresureMap)
{
    if (_action == "hit") {
        this.playEffectSound("snd_plane_transition");
    }
    else {
        this.playEffectSound("snd_plane_transition_2");
    }
    this.stopBGMSound();
    let spine = this.$spineContainer.borrow("s346_ui_pig_air");
    this.$slot.popup_container.addChild(spine);
    spine.setAnimation(_action);
    spine.addAnimationComplete(
        _action, (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    if (spine && spine.parent) {
                        spine.parent.removeChild(spine);
                        spine = null;
                    }
                    this.$spinProcessQueue.next(0);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
    if (isTresureMap) {
        co(function * () {
            try {
                yield wait(1000);
                if (this._is_destroy) {
                    return;
                }
                this.playBGMSound("bgm_pick_" + type);
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this));
        co(function * () {
            try {
                yield wait(2000);
                if (this._is_destroy) {
                    return;
                }
                this.makeInfoPig(true);
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this));
    }
}

setChangeBG(isTresureMap, type, _type)
{
    this.removeBottomUIEffect();
    this._initAllAnimations();
    if (isTresureMap) {
        if (this.pickRetrigger) {
            this.removeEndPopup();
        }
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            this.removeFixedSymbolForBonus();
            this.removeOpenBox();
            this.removeOpenFrame();
        }
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            this.replaceIconLowForPyramid();
            this.removeFixedSymbolForPyramid();
        }
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            this.replaceIconLowForOriental();
            this.removeFixedSymbolForOriental();
        }
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            this.replaceIconLowForAtlantis();
            this.removeFixedSymbolForAtlantis();
            this.removePayBoxForAtlantis();
        }
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            this.removeChoiceMap();
            this.removeCompass();
            this.removeFixedSymbolForBonus();
            this.removeOpenBox();
            this.removeOpenFrame();
        }
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            this.removeChoiceMap();
            this.removeCompass();
        }
        this.removeStay();
        this.spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
        this.setSlotMode("bonus");
        if (this.$uiKey) {
            this.$uiKey.setAnimation("loop", true);
        }
        this.setBonustime(true);
        this.setAvgTotalBet();
        // this.setOnlyBonusPotPay(false);
        this.$bottomUI.setGoodLuck();
        this.makeTreasureBox(type);
        this.setReelVisible(-1);
        this.removeUiScroll();
    }
    else {
        this.spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
        this.setSlotMode("normal");
        this.win_number = this._incremental_bonus_pot;
        this.setBonustime(false);
        this.makeUiScroll();
        this.removeKey();
        this._playScatterHitAnimationForNormal("stay", true);
        this.removeTresureBox();
        this.stopBGMSound();
        this._changeTotalBet();
        this.playBGMSound("bgm_normal_reel");
    }
    this.$slot.bg.gotoAndStop(_type);
    this.$slot.reel_bg.gotoAndStop(_type);
}

onScrollMouseOverListner()
{
    if (this._bSpin || this.isClick || this._bSpinAuto) {
        this.$uiScroll.buttonMode = false;
        return;
    }
    this.$uiScroll.buttonMode = true;
    this.$uiScroll.setAnimation("over");
}

onScrollMouseOutListner()
{
    if (this._bSpin || this.isClick || this._bSpinAuto) return;
    this.$uiScroll.setAnimation("stay", true);
}

onScrollPointerdownListner()
{
    if (this._bSpin || this.isClick || this._bSpinAuto) return;
    this.$uiScroll.setAnimation("open_1");
    this.playEffectSound("snd_button");
}

onScrollPointerupListner()
{
    if (this._bSpin || this.isClick || this._bSpinAuto) return;
    this.openMap = true;
    this.isClick = true;
    this.$uiScroll.setAnimation("open_2");
    this.$uiScroll.addAnimation("stay", true);
    this.makeChoiceMap();
}

onChoiceMapMouseOverListner(type)
{
    if (this._bSpin || this.isClick) return;
    this.$uiChoiceMap.setAnimation("bonus_" + type + "_over");
}

onChoiceMapMouseOutListner()
{
    if (this._bSpin || this.isClick) return;
    this.$uiChoiceMap.setAnimation("info_on");
}

onChoiceMapPointerupListner(type)
{
    if (this._bSpin || this.isClick) return;
    this.isClick = true;
    this.$uiChoiceMap.setAnimation("bonus_" + type + "_info_up");
    co(function * () {
        try {
            yield wait(500);
            if (this._is_destroy) {
                return;
            }
            this.removeUiTresureMap();
            this.$uiTresureMap = this.$spineContainer.borrow("s346_ui_bonus_" + type + "_map");
            this.$slot.eff_container.addChild(this.$uiTresureMap);
            this.makeTreasureBox(type, true);
            if (this.$exitButton) {
                this._makeButtonForTresureMapExit(this.$exitButton);
            }
            if (this.$uiChoiceMap) {
                this.$uiChoiceMap.visible = false;
            }
            if (this.$infoPig) {
                this.$infoPig.setAnimation("in_loop2", true)
            }
            this.isClick = false;
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

onChoiceMapPointerdownListner(type)
{
    if (this._bSpin || this.isClick) return;
    // this.isClick = true;
    this.playEffectSound("snd_button");
    this.$uiChoiceMap.setAnimation("bonus_" + type + "_info_down");
}

onPaytableMouseOverListner(index)
{
    if (this._bSpin || this.isClick) return;
    this.$paytalbeInfo[index].setAnimation("over");
}

onPaytableMouseOutListner(index)
{
    if (this._bSpin || this.isClick) return;
    this.$paytalbeInfo[index].setAnimation("on");
}

onPaytablePointerdownListner()
{
    if (this._bSpin || this.isClick) return;
    this.playEffectSound("snd_button");
}

onPaytablePointerupListner(index)
{
    if (this._bSpin || this.isClick) return;
    this.$paytalbeInfo[index].setAnimation("hit");
    let page = 0;
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
    if (this.$paytable) {
        if (this.$paytable.visible) {
            this.$paytable.hide();
        }
        else {
            this.$paytable.show();
            this.addChild(this.$paytable);
            this.$paytable.paytable_current_index = page this.$paytable.$main.paytable.gotoAndStop(page);
        }
    }
    else {
        let _url = this.getPaytableResUrl();
        let _paytableResource = {
            name : StringUtils.formatExt(DUG.common.settings.SLOT_PAYTABLE_CLASS_HOLDER_FORMAT, {slot_type : this._slotConfig.slot_type}),
            url : _url,
            metadata : {
                path : 'relative',
                type : "animate",
                slot_type : this._slotConfig.slot_type,
                version : this._slotConfig.version,
            }
        };
        let _simpleLoader = CResourceSimpleLoader.getInstance();
        _simpleLoader.clear();
        _simpleLoader.addResource(_paytableResource);
        _simpleLoader.load();
        this.eventListener.on(
            _simpleLoader, 'complete', this._onCompleteLoadOpenBigWinPopupBindMe = function() {
                this.eventListener.off(_simpleLoader, 'complete', this._onCompleteLoadOpenBigWinPopupBindMe);
                this._createPaytable();
                // 페이테이블 아래로 이벤트가 전이되지 않도록
                this.$paytable.interactive = true;
                this.$paytable.y = this.$slot.y + this.$slotContainer.y * this.$slotContainer.scale._y;
                this.$paytable.x = this.$slot.x + this.$slotContainer.x * this.$slotContainer.scale._x;

                if (this._layouts.paytable && this._layouts.paytable.adjust_x) {
                    this.$paytable.x += this._layouts.paytable.adjust_x;
                }
                if (this._layouts.paytable && this._layouts.paytable.adjust_y) {
                    this.$paytable.y += this._layouts.paytable.adjust_y;
                }

                this.$paytable.scale._x = this.$slotContainer.scale._x;
                this.$paytable.scale._y = this.$slotContainer.scale._y;
                // this.$slotContainer.addChild(this.$paytable);
                this.addChild(this.$paytable);
                this.eventListener.on(
                    this.$paytable, 'initialized', this.onInitializedPaytableBindMe = function() {
                        this.eventListener.off(this.$paytable, 'initialized', this.onInitializedPaytableBindMe);
                        this.$paytable.setPaytablePayout(this._unitBet);
                        this.$paytable.paytable_current_index = page this.$paytable.$main.paytable.gotoAndStop(page);
                    }.bind(this, page));
            }.bind(this, page));
    }
}

onExitMouseOverListner()
{
    if (this._bSpin || this.isClick) return;
    this.$exitButton.setAnimation("over");
}

onExitMouseOutListner()
{
    if (this._bSpin || this.isClick) return;
    this.$exitButton.setAnimation("on");
}

onExitPointerdownListner()
{
    if (this._bSpin || this.isClick) return;
    this.playEffectSound("snd_button");
    this.$exitButton.setAnimation("hit_1");
}

onExitForChoiceMapPointerupListner()
{
    if (this._bSpin || this.isClick) return;
    this.removePaytableInfo();
    this.isClick = true;
    this.$exitButton.setAnimation("hit_2");
    this.$exitButton.addAnimationComplete(
        "hit_2", (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.removeExitButton();
                    this.$uiChoiceMap.setAnimation("out");
                    this.$uiChoiceMap.addAnimationComplete(
                        "out", (entry) = > {
                            co(function * () {
                                try {
                                    yield wait(10);
                                    if (this._is_destroy) {
                                        return;
                                    }
                                    this.removeChoiceMap();
                                    this.$uiScroll.buttonMode = true;
                                    this.$uiScroll.interactive = true;
                                    this.isClick = false;
                                    this.openMap = false;
                                }
                                catch (error) {
                                    DUG.log(error);
                                }
                            }.bind(this));
                        });
                    this.removeInfoPig(true, false);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}

onExitForTresureMapPointerupListner()
{
    if (this._bSpin || this.isClick) return;
    this.isClick = true;
    this.$exitButton.setAnimation("hit_2");
    this.$exitButton.addAnimationComplete(
        "hit_2", (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    if (this.$exitButton) {
                        this._makeButtonForChoiceMapExit(this.$exitButton);
                    }
                    if (this.$uiChoiceMap) {
                        this.$uiChoiceMap.visible = true;
                        this.$uiChoiceMap.setAnimation("info_on");
                    }
                    if (this.$infoPig) {
                        this.$infoPig.setAnimation("in_loop", true);
                    }
                    this.removeTresureBox();
                    this.removeUiTresureMap();
                    this.isClick = false;
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}

/**
 * 버튼 ui 생성
 * @protected
 */
_makeButtonForScroll($button)
{
    $button.buttonMode = true;
    $button.interactive = true;
    $button.mouseover = this.onScrollMouseOverListner.bind(this);
    $button.mouseout = this.onScrollMouseOutListner.bind(this);
    $button.mousedown = this.onScrollPointerdownListner.bind(this);
    $button.mouseup = this.onScrollPointerupListner.bind(this);
}

/**
 * 버튼 ui 생성
 * @protected
 */
_makeButtonForChoiceMapExit($button)
{
    $button.buttonMode = true;
    $button.interactive = true;
    $button.mouseover = this.onExitMouseOverListner.bind(this);
    $button.mouseout = this.onExitMouseOutListner.bind(this);
    $button.mousedown = this.onExitPointerdownListner.bind(this);
    $button.mouseup = this.onExitForChoiceMapPointerupListner.bind(this);
}

/**
 * 버튼 ui 생성
 * @protected
 */
_makeButtonForChoiceMap($button, type)
{
    $button.buttonMode = true;
    $button.interactive = true;
    $button.mouseover = this.onChoiceMapMouseOverListner.bind(this, type);
    $button.mouseout = this.onChoiceMapMouseOutListner.bind(this);
    $button.mouseup = this.onChoiceMapPointerupListner.bind(this, type);
    $button.mousedown = this.onChoiceMapPointerdownListner.bind(this, type);
}

/**
 * 버튼 ui 생성
 * @protected
 */
_makeButtonForPaytalble($button, index)
{
    $button.buttonMode = true;
    $button.interactive = true;
    $button.mouseover = this.onPaytableMouseOverListner.bind(this, index);
    $button.mouseout = this.onPaytableMouseOutListner.bind(this, index);
    $button.mouseup = this.onPaytablePointerupListner.bind(this, index);
    $button.mousedwon = this.onPaytablePointerdownListner.bind(this, index);
}

/**
 * 버튼 ui 생성
 * @protected
 */
_makeButtonForTresureMapExit($button)
{
    $button.buttonMode = true;
    $button.interactive = true;
    $button.mouseover = this.onExitMouseOverListner.bind(this);
    $button.mouseout = this.onExitMouseOutListner.bind(this);
    $button.mousedown = this.onExitPointerdownListner.bind(this);
    $button.mouseup = this.onExitForTresureMapPointerupListner.bind(this);
}

playCompassAnimation()
{
    let _type = String.fromCharCode(this.pickType + 96);
    this.playEffectSound("snd_compass_spin");
    this.$compass.setAnimation("bonus_" + _type);
    this.$compass.addAnimationComplete(
        "bonus_" + _type, (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.playEffectSound("snd_compass_stop");
                    this.$uiChoiceMap.setAnimation("bonus_" + _type);
                    yield wait(700);
                    if (this._is_destroy) {
                        return;
                    }
                    this.changeBG(true, _type);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this, _type));
        });
}

createTresureBox(options)
{
    return new CSlot346PiggyAdventureTresureBox(options);
}

/*
 * 보물상자
 */
makeTreasureBox(_type, _info = false)
{
    this.removeTresureBox();
    this.$tresureBox = this.create1DArray(30, null);
    this.remainPickInfo = [];
    let pickIndex = _type.charCodeAt(0) - 97;
    this.$uiTresureBox = this.$spineContainer.borrow("s346_ui_box_set");
    this.$uiTresureBox.setAnimation("on");
    this.$slot.eff_container.addChild(this.$uiTresureBox);
    if (_info) {
        this.$uiTresureBox.scale.x = 0.9;
        this.$uiTresureBox.scale.y = 0.9;
    }
    for (let index = 0; index < 30; index++) {
        let _pay = this.pickResultInfo[`$ { pickIndex + 1 }`][`$ { index }`];
        let _slot = this.$uiTresureBox.getSlotContainer(`num_$ { index + 1 }`);
        if (!_pay || _pay == undefined || _pay.length == 0) {
            this.remainPickInfo.push(index);
        }
        this.$tresureBox[index] = this.createTresureBox({
            type : _type,
            isPick : this.isPick ? true : false,
            pay : (_pay && _pay.length > 0) ? _pay : "",
            index : index,
            slot : this,
            container : _slot,
            $spineContainer : this.$spineContainer
        });
        this.$tresureBox[index].initBox();
    }
}

removePaytableInfo()
{
    for (let index = 0; index < this.$paytalbeInfo.length; index++) {
        if (this.$paytalbeInfo[index] && this.$paytalbeInfo[index].parent) {
            this.$paytalbeInfo[index].parent.removeChild(this.$paytalbeInfo[index]);
            this.$paytalbeInfo[index].destroy({children : true});
            this.$paytalbeInfo[index] = null;
        }
    }
}

removeUiTresureMap()
{
    if (this.$uiTresureMap && this.$uiTresureMap.parent) {
        this.$uiTresureMap.parent.removeChild(this.$uiTresureMap);
        this.$uiTresureMap.destroy({children : true});
        this.$uiTresureMap = null;
    }
}

removeInfoPig(animation = false, isTresureMap = false)
{
    if (animation) {
        if (isTresureMap) {
            this.$infoPig.setAnimation("out");
            this.$infoPig.addAnimationComplete(
                "out", (entry) = > {
                    co(function * () {
                        try {
                            yield wait(0);
                            if (this._is_destroy) {
                                return;
                            }
                            this.removeInfoPig();
                        }
                        catch (error) {
                            DUG.log(error);
                        }
                    }.bind(this));
                });
        }
        else {
            this.$infoPig.setAnimation("in_out");
            this.$infoPig.addAnimationComplete(
                "in_out", (entry) = > {
                    co(function * () {
                        try {
                            yield wait(0);
                            if (this._is_destroy) {
                                return;
                            }
                            this.removeInfoPig();
                        }
                        catch (error) {
                            DUG.log(error);
                        }
                    }.bind(this));
                });
        }
    }
    else {
        if (this.$infoPig && this.$infoPig.parent) {
            this.$infoPig.parent.removeChild(this.$infoPig);
            this.$infoPig.destroy({children : true});
            this.$infoPig = null;
        }
    }
}

removeTresureBox()
{
    for (let index = 0; index < this.$tresureBox.length; index++) {
        if (this.$tresureBox[index] && this.$tresureBox[index].parent) {
            this.$tresureBox[index].parent.removeChild(this.$tresureBox[index]);
            this.$tresureBox[index].destroy({children : true});
            this.$tresureBox[index] = null;
        }
    }
    if (this.$uiTresureBox && this.$uiTresureBox.parent) {
        this.$uiTresureBox.parent.removeChild(this.$uiTresureBox);
        this.$uiTresureBox.destroy({children : true});
        this.$uiTresureBox = null;
    }
}

removeCountDown()
{
    if (this.$countDown && this.$countDown.parent) {
        this.$countDown.parent.removeChild(this.$countDown);
        this.$countDown.destroy({children : true});
        this.$countDown = null;
    }
}

removeUiScroll()
{
    if (this.$uiScroll && this.$uiScroll.parent) {
        this.$uiScroll.parent.removeChild(this.$uiScroll);
        this.$uiScroll.destroy({children : true});
        this.$uiScroll = null;
    }
}

removeExitButton()
{
    if (this.$exitButton && this.$exitButton.parent) {
        this.$exitButton.parent.removeChild(this.$exitButton);
        this.$exitButton.destroy({children : true});
        this.$exitButton = null;
    }
}

removeCompass()
{
    if (this.$compass && this.$compass.parent) {
        this.$compass.parent.removeChild(this.$compass);
        this.$compass.destroy({children : true});
        this.$compass = null;
    }
}

removeChoiceMap()
{
    if (this.$uiChoiceMap && this.$uiChoiceMap.parent) {
        this.$uiChoiceMap.parent.removeChild(this.$uiChoiceMap);
        this.$uiChoiceMap.destroy({children : true});
        this.$uiChoiceMap = null;
    }
}

removeKey()
{
    if (this.$uiKey && this.$uiKey.parent) {
        this.$uiKey.parent.removeChild(this.$uiKey);
        this.$uiKey.destroy({children : true});
        this.$uiKey = null;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 함수
////////////////////////////////////////////////////////////////////////////////////////////
onStartSuperFreespin()
{
    this.superFreespinStart = false;
    this.isSuperFreespin = true;
    this.isPick = false;
    this.pickEnd = false;
    this._incremental_bonus_pot = 0;
    this._incremental_bonus_pot_end = 0;
    this.bonuspot_number = 0;
    this.orientalTotalAward = 0;
    this.orientalTotalAwardPre = 0;
    this._changeTotalBet();
    this.setOnlyBonusPotPay(false);
    this.setAvgTotalBet();
    this.$bottomUI.setGoodLuck();
    this.$spinProcessQueue.next(100);
}

onStartSuperFreespin1()
{
    this.playSuperStart();
}

onStartSuperFreespin2()
{
    this.firstSpinForSuper = true;
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        this.$payBoxNumForAtlantis = this.$spineContainer.borrow("s346_ui_bonus_b_pay");
        this.$slot.eff_container3.addChild(this.$payBoxNumForAtlantis);
        this.$payBoxNumForAtlantisBig = this.$spineContainer.borrow("s346_ui_bonus_b_pay");
        this.$slot.eff_container3.addChild(this.$payBoxNumForAtlantisBig);
        this.$payBoxTextForAtlantis = this.create1DArray(this.paylinesForAtlantis.length, null);
        this.$payBoxTextForAtlantisBig = this.create1DArray(this.paylinesForAtlantis.length, null);
        this.$payHitAniForAtlantis = this.create1DArray(this.paylinesForAtlantis.length, null);
        this.linePayForAtlantis = this.create1DArray(this.paylinesForAtlantis.length, 0);
        this.superFreespinStartAnimationForAtlantis();
        this.$spinProcessQueue.next(2500);
    }
    else {
        this.$spinProcessQueue.next(100);
    }
}

onEndSuperFreespin()
{
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        if (this.$payoutSpine) {
            this.$payoutSpine.setAnimation("out");
            this.$payoutSpine.addAnimationComplete(
                "out", (entry) = > {
                    co(function * () {
                        try {
                            yield wait(0);
                            if (this._is_destroy) {
                                return;
                            }
                            this.removeStay();
                            this.removePayoutSpine();
                            this.playEndPopup();
                        }
                        catch (error) {
                            DUG.log(error);
                        }
                    }.bind(this));
                });
        }
    }
    else {
        this.playEndPopup();
    }
}

onEndSuperFreespin1()
{
    this.$spinProcessQueue.next(0);
}

onEndSuperFreespin2()
{
    this.isSuperFreespin = false;
    this.superFreespinEnd = false;
    this._writeSlotActionLog({
        category_info : '',
        action : 'SUPER_BONUS_EXIT',
        action_info : '',
        isplaynow : 1
    });
    if (this.pickCount > 0) {
        this.$spinProcessQueue.next(0);
    }
    else {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            this.setNormal();
            this.$bottomUI.setSpinAgain();
            this.superTotalAward = 0;
        }
        this.$spinProcessQueue.next(100);
    }
}

setNormal()
{
    this._initAllAnimations();
    this.removeEndPopup();
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
        this.removeFixedSymbolForBonus();
        this.removeOpenBox();
        this.removeOpenFrame();
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        this.replaceIconLowForPyramid();
        this.removeFixedSymbolForPyramid();
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        this.replaceIconLowForOriental();
        this.removeFixedSymbolForOriental();
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        this.replaceIconLowForAtlantis();
        this.removeFixedSymbolForAtlantis();
        this.removePayBoxForAtlantis();
    }
    this.removeStay();
    this.removeBottomUIEffect();
    this.spinType = CSlot346PiggyAdventure.SPIN_TYPE.NORMAL;
    this.setBottomUI(true);
    this.setSlotMode("normal");
    this._playScatterHitAnimationForNormal("stay", true);
    this.removeKey();
    this.makeUiScroll();
    this.stopBGMSound();
    this._changeTotalBet();
    this.playBGMSound("bgm_normal_reel");
}

playEndPopup()
{
    this.removeEndPopup();
    this.stopBGMSound();
    let _type = String.fromCharCode(this.pickType + 96);
    this.playEffectSound("snd_result_" + _type);
    let totalAward = this.totalAward(true);
    this.$endPopup = this.$spineContainer.borrow("s346_ui_bonus_" + _type + "_closing");
    this.$endPopup.setAnimation("hit");
    this.$slot.popup_container.addChild(this.$endPopup);
    let _text = this.$endPopup.addBitmapText("num", {size : 74, font : "s346_num_congratulations", align : "center", alignY : "center", alignY : "center"}, this.eventListener);
    _text.text = CMiscUtils.moneyFormatter(totalAward);
    _text.setAutoAlign();
    this.$endPopup.addAnimationComplete(
        "hit", (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.$spinProcessQueue.next(0);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
    this.playEffectSpine("s346_ui_payout", this.$slot.popup_container, "hit");
}

getMultipotForSuper(pay)
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

playSuperStart()
{
    this.removeStay();
    this.stopBGMSound();
    this.playBGMSound("bgm_super_" + String.fromCharCode(this.pickType + 96));
    switch (this.pickType) {
        case 1:
            this.playSlotStayForPyramid();
            break;
        case 2:
            this.playSlotStayForAtlantis();
            break;
        case 3:
            this.playSlotStayForOriental();
            break;
        default:
            break;
    }
}

playSlotStayForPyramid()
{
    let delay = 666;
    let action = "appear";
    this.playEffectSound("snd_intro_super_a");
    this.$staySpine = this.$spineContainer.borrow("s346_ui_bonus_a_pig");
    this.$staySpine.setAnimation(action);
    this.$staySpine.addAnimation("loop", true);
    this.$slot.eff_container2.addChild(this.$staySpine);
    co(function * () {
        try {
            yield wait(delay);
            if (this._is_destroy) {
                return;
            }
            this.playSuperInPopupForPyramid();
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this, delay));
}

playSuperInPopupForPyramid()
{
    this.setSuperIn("a");
    this.removeOpenPopup();
    this.playBlackLayer("in", this.$slot.black);
    this.playEffectSpine("s346_ui_bonus_a_reel_eff", this.$slot.eff_container, "hit");
    this.$openPopup = this.$spineContainer.borrow("s346_ui_bonus_a_bonus");
    this.$openPopup.setAnimation("hit");
    this.$slot.popup_container.addChild(this.$openPopup);
    this.$openPopup.addAnimationComplete(
        "hit", (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.playEffectSound("snd_pig_a_1");
                    this.removeOpenPopup();
                    this.removeBlackLayer(true);
                    this.$spinProcessQueue.next(0);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}

playSlotStayForAtlantis()
{
    let delay = 1000;
    let action = "appear";
    this.playEffectSound("snd_intro_super_b");
    this.$staySpine = this.$spineContainer.borrow("s346_ui_bonus_b_pig");
    this.$staySpine.setAnimation(action);
    this.$slot.eff_container2.addChild(this.$staySpine);
    co(function * () {
        try {
            yield wait(delay);
            if (this._is_destroy) {
                return;
            }
            this.playSuperInPopupForAtlantis();
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this, delay));
}

playSuperInPopupForAtlantis()
{
    this.replaceIconLowForAtlantis();
    this.setSuperIn("b");
    this.removeOpenPopup();
    this.$openPopup = this.$spineContainer.borrow("s346_ui_bonus_b_bonus");
    this.$openPopup.setAnimation("hit");
    this.$slot.popup_container.addChild(this.$openPopup);
    this.$openPopup.addAnimationComplete(
        "hit", (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.playPayoutSpineForAtlantis();
                    this.removeOpenPopup();
                    this.$spinProcessQueue.next(0);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}

playPayoutSpineForAtlantis()
{
    this.$payoutSpine = this.$spineContainer.borrow("s346_ui_bonus_b_pearl");
    this.$slot.eff_container.addChild(this.$payoutSpine);
    if (this.isRecovery) {
        this.$payoutSpine.setAnimation("stay", true);
    }
    else {
        this.$payoutSpine.setAnimation("appear");
        this.$payoutSpine.addAnimation("stay", true);
    }
}

playSlotStayForOriental()
{
    this.replaceIconLowForOriental();
    let delay = 630;
    let action = "in";
    this.playEffectSound("snd_intro_super_c");
    this.$staySpine = this.$spineContainer.borrow("s346_ui_bonus_c_pig");
    this.$staySpine.setAnimation(action);
    this.$slot.eff_container.addChild(this.$staySpine);
    co(function * () {
        try {
            yield wait(delay);
            if (this._is_destroy) {
                return;
            }
            this.playSuperInPopupForOriental();
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this, delay));
    this.$staySpine.addAnimation("side_in");
    this.$staySpine.addAnimation("side_loop", true);
    co(function * () {
        try {
            yield wait(4000);
            if (this._is_destroy) {
                return;
            }
            this.playEffectSpine("s346_ui_bonus_c_reel_eff", this.$slot.eff_container, "start");
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

playSuperInPopupForOriental()
{
    this.playBlackLayer("in", this.$slot.black);
    this.setSuperIn("c");
    this.removeOpenPopup();
    this.$openPopup = this.$spineContainer.borrow("s346_ui_bonus_c_bonus");
    this.$openPopup.setAnimation("hit");
    this.$slot.eff_container2.addChild(this.$openPopup);
    this.$openPopup.addAnimationComplete(
        "hit", (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    this.removeOpenPopup();
                    this.removeBlackLayer(true);
                    this.$spinProcessQueue.next(0);
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}

setSuperIn(type)
{
    this._setResultIconsAlpha(1);
    this._releaseAnimationIcons();
    this.hideAllPaylines();
    this.setBonustime(false);
    this.setSlotMode("bonus");
    this.setResultIconVisible();
    this.removeTresureBox();
    this.setOnlyBonusPotPay(false);
    if (this.pickCount == 0) {
        this.removeKey();
    }
    else {
        if (this.$uiKey) {
            this.$uiKey.setAnimation("bonus_loop", true);
        }
    }
    this.$bottomUI.setGoodLuck();
    let _type = "";
    switch (type) {
        case "a":
            _type = "pyramid_super";
            this.superFreespinTotalCount = 7;
            break;
        case "b":
            _type = "atlantis_super";
            this.superFreespinTotalCount = 10;
            break;
        case "c":
            _type = "oriental_super";
            this.superFreespinTotalCount = 5;
            break;
        default:
            break;
    }
    this.writeAcionLogForSuper();
    this.setFreespinLeftcount(this.superFreespinRemainCount, this.superFreespinTotalCount);
    this.$slot.bg.gotoAndStop(_type);
    this.$slot.reel_bg.gotoAndStop(_type);
}

writeAcionLogForSuper()
{
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        this._writeSlotActionLog({
            category_info : '',
            action : 'SUPER_BONUS_ENTER: ' + this.superFreespinTotalCount,
            action_info : '1-king tut',
            isplaynow : 1
        });
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        this._writeSlotActionLog({
            category_info : '',
            action : 'SUPER_BONUS_ENTER: ' + this.superFreespinTotalCount,
            action_info : '2-neptune',
            isplaynow : 1
        });
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        this._writeSlotActionLog({
            category_info : '',
            action : 'SUPER_BONUS_ENTER: ' + this.superFreespinTotalCount,
            action_info : '3-ancient',
            isplaynow : 1
        });
    }
}

removeStay()
{
    if (this.$staySpine && this.$staySpine.parent) {
        this.$staySpine.parent.removeChild(this.$staySpine);
        this.$staySpine.destroy({children : true});
        this.$staySpine = null;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 피라미드 함수
////////////////////////////////////////////////////////////////////////////////////////////
_createSlotReelWrapperForPyramid(instance, options)
{
    options.onBounceStopSpin = this._onBounceStopSpinForPyramid.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    let randomStart = CMiscUtils.getRandomIntInclusive(0, options.reelArray.length - 1);
    options.startPosition = randomStart;
    options.spineContainer = this.$spineContainer;
    options.findIndexFunc = this.findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventurePyramidReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
_onBounceStopSpinForPyramid($slotReelWrapper)
{
    const reelIndex = $slotReelWrapper.reelIndex;
    // 릴 스핀 종료 재생
    let _resultIcons = $slotReelWrapper.getResultIcons();
    let scatterAppear = false;
    for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
        if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
            continue;
        }
        let $icon = _resultIcons[rowIndex];
        let _iconId = $icon._dug_icon_id;
        if (!this.isPyramidScatter(_iconId)) {
            continue;
        }
        scatterAppear = true;
        $icon.visible = false;
        let spineName = "s346_icon_bonus_a_high";
        if (_iconId == 210) {
            spineName = "s346_icon_bonus_a_wild";
        }
        let action = "lock";
        if (_iconId == 210 || (_iconId == 203 && reelIndex == 0)) {
            action = "appear";
        }
        this.$fixedSymbolForPyramid[reelIndex][rowIndex] = this.$spineContainer.borrow(spineName);
        this.$slot.layer_reel_frame_pyramid.addChild(this.$fixedSymbolForPyramid[reelIndex][rowIndex]);
        this.$fixedSymbolForPyramid[reelIndex][rowIndex].setAnimation(action);
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
        this.$fixedSymbolForPyramid[reelIndex][rowIndex].adjustPosition($reel, rowIndex, 1);
        this.$fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id = _iconId;
        this.$fixedSymbolForPyramid[reelIndex][rowIndex]._reelIndex = reelIndex;
        this.$fixedSymbolForPyramid[reelIndex][rowIndex]._rowIndex = rowIndex;
        if (reelIndex != 0) {
            this.setIconTextForPyramid(this.$fixedSymbolForPyramid[reelIndex][rowIndex], _iconId);
            if (action == "lock" && this.$fixedSymbolForPyramid[reelIndex][rowIndex].$attach) {
                this.$fixedSymbolForPyramid[reelIndex][rowIndex].$attach.setAnimation(action);
            }
        }
    }
    this.playEffectSound("snd_reel_stop");
    if (scatterAppear) {
        this.playEffectSound("snd_symbol_appear_a");
    }
}

pyramidProc()
{
    this.updateStaySymbolForPyramid();
    this.$spinProcessQueue.next(300);
}

pyramidPayoutProc()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
                let $icon = this.$fixedSymbolForPyramid[reelIndex][rowIndex];
                let action = "on";
                $icon.setAnimation(action);
                if ($icon.$attach) {
                    $icon.$attach.setAnimation(action);
                }
            }
        }
    }
    this.setOnlyBonusPotPay(true);
    let _totalAward = this._currentResult.spinResult.payout.totalAward;
    let kind = this.getPayoutBeforeEffectKind(_totalAward / this._avgTotalBet);
    if (kind != = 'none') {
        this.$bottomUI.playPayoutWinEffect(kind);
        this.playPayoutBeforeEffectSound(kind);
    }
    this.$spinProcessQueue.next(1000);
}

pyramidPayoutProc1()
{
    if (this._currentResult.spinResult.payout.lines) {
        this.totalPayTemp = 0;
        this.$staySpine.setAnimation("hit", true);
        this.stopBGMSound();
        this.payoutType = "a1";
        this.playBGMSound("bgm_payout_a1");
        co(function * () {
            try {
                let _hitIcons = this._findAllHitIconsForPyramid();
                let totalPay = 0;
                for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
                    let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
                    let icons = $slotReelWrapper.getResultIcons();
                    for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                        if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
                            if (!ArrayUtils.contains(_hitIcons, this.$fixedSymbolForPyramid[reelIndex][rowIndex])) {
                                this.$fixedSymbolForPyramid[reelIndex][rowIndex].alpha = 0.2;
                                this.$fixedSymbolForPyramid[reelIndex][rowIndex].setAnimation("on");
                                icons[rowIndex].visible = false;
                            }
                        }
                    }
                }
                for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
                    let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
                    for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                        if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
                            if (!ArrayUtils.contains(_hitIcons, this.$fixedSymbolForPyramid[reelIndex][rowIndex])) {
                                continue;
                            }
                            let iconId = this.$fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id;
                            if (iconId == 203) {
                                let delay = 500;
                                let action = "pay";
                                if (this.fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
                                    action = "multipot" delay = 3300;
                                }
                                this.$fixedSymbolForPyramid[reelIndex][rowIndex].parent.addChild(this.$fixedSymbolForPyramid[reelIndex][rowIndex]);
                                this.$fixedSymbolForPyramid[reelIndex][rowIndex].setAnimation(action);
                                this.$fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id = iconId;
                                this.$fixedSymbolForPyramid[reelIndex][rowIndex]._reelIndex = reelIndex;
                                this.$fixedSymbolForPyramid[reelIndex][rowIndex]._rowIndex = rowIndex;
                                if (this.$fixedSymbolForPyramid[reelIndex][rowIndex].$attach) {
                                    this.$fixedSymbolForPyramid[reelIndex][rowIndex].$attach.setAnimation(action);
                                }
                                this.playPayoutEffectIconForPyramid(reelIndex, rowIndex);
                                yield wait(delay);
                                if (this._is_destroy) {
                                    return;
                                }
                            }
                        }
                    }
                }
                this.stopEffectSound("snd_payout_" + this.payoutType);
                this.playEffectSound("snd_payout_" + this.payoutType + "_end");
                this._incremental_bonus_pot_end = 0;
                this._incremental_bonus_pot = this.totalPayTemp;
                this.bonuspot_number = this.totalPayTemp;
                this.$spinProcessQueue.next(2000);
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this));
    }
}

pyramidPayoutProc2()
{
    this.removeMultipotPopup();
    if (this._currentResult.spinResult.payout.lines) {
        this.showHitPaylines();
        this.playHidePaylineProcess();
        let _hitIcons = this._findAllHitIconsForPyramid();
        for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
            for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
                    let $icon = this.$fixedSymbolForPyramid[reelIndex][rowIndex];
                    if (!ArrayUtils.contains(_hitIcons, $icon)) {
                        continue;
                    }
                    let action = "hit";
                    $icon.setAnimation(action, true);
                    if ($icon.$attach) {
                        $icon.$attach.setAnimation(action, true);
                    }
                }
            }
        }
        let _init = this.totalPayTemp;
        let _totalAward = this._currentResult.spinResult.payout.totalAward;
        let _duration = this.calcNewCoinIncreaseTime(_totalAward - _init);
        this.payoutType = "a2";
        this.playEffectSoundWithOption("snd_payout_" + this.payoutType, "snd_payout_" + this.payoutType, true);
        this.playPayoutIncrement({
            delay : 2000,
            duration : _duration,
            init : _init,
            result : _totalAward,
            payoutType : 'update_bonuspot'
        });
    }
}

playPayoutEffectIconForPyramid(reelIndex, rowIndex)
{
    co(function * () {
        try {
            let delay = 300;
            if (this.fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
                delay = 500;
                yield wait(500);
                if (this._is_destroy) {
                    return;
                }
                this.playEffectSound("snd_text_popup_a");
            }
            else {
                this.playEffectSound("snd_count_a");
            }
            yield wait(delay);
            if (this._is_destroy) {
                return;
            }
            this.totalPayTemp += this.fixedPayoutForSuper[reelIndex][rowIndex] * this._avgTotalBet / 10;
            this.bonuspot_number = this.totalPayTemp;
            if (this.fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
                this.playMultipotPopup(this.fixedPayoutForSuper[reelIndex][rowIndex], true);
            }
            if (this.$bottomUI) {
                this.$bottomUI.playPayoutEffect();
            }
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

playHidePaylineProcess()
{
    this.$hidePaylineProcessQueue.killAndDone();
    this.$hidePaylineProcessQueue.add(
        this.hidePaylineBindMe
            ? this.hidePaylineBindMe
            : (this.hidePaylineBindMe =
                   this.hideAllPaylines.bind(this)));
    let delay = 1000;
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
        delay = 2000;
    }
    this.$hidePaylineProcessQueue.next(delay);
}

calcCoinIncreaseTimeForPyramid()
{
    let _duration = 0;
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
                let $icon = this.$fixedSymbolForPyramid[reelIndex][rowIndex];
                let iconId = $icon._dug_icon_id;
                if (iconId == 203) {
                    if (this.fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
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

updateStaySymbolForPyramid()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            if (this.$fixedSymbolForPyramid[reelIndex][rowIndex]) {
                let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
                let $icon = this.$fixedSymbolForPyramid[reelIndex][rowIndex];
                $icon.setAnimation("loop", true);
                this.$slot.layer_reel_frame_pyramid.addChild($icon);
                $icon.adjustPosition($reel, rowIndex, 1);
                this.setIconTextForPyramid($icon, $icon._dug_icon_id);
            }
        }
    }
}

recoveryProcForPyramid()
{
    this.getFeaturedInfo();
    this.$staySpine = this.$spineContainer.borrow("s346_ui_bonus_a_pig");
    this.$staySpine.setAnimation("loop", true);
    this.$slot.eff_container2.addChild(this.$staySpine);
    this.replaceIconLowForPyramid();
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            let $icon = _resultIcons[rowIndex];
            let _iconId = this.fixedPlaylineForSuper[reelIndex][rowIndex];
            if (!this.isPyramidScatter(_iconId)) {
                continue;
            }
            $icon.visible = false;
            let spineName = "s346_icon_bonus_a_high";
            if (_iconId == 210) {
                spineName = "s346_icon_bonus_a_wild";
            }
            this.$fixedSymbolForPyramid[reelIndex][rowIndex] = this.$spineContainer.borrow(spineName);
            this.$slot.layer_reel_frame_pyramid.addChild(this.$fixedSymbolForPyramid[reelIndex][rowIndex]);
            this.$fixedSymbolForPyramid[reelIndex][rowIndex].adjustPosition($reel, rowIndex, 1);
            this.$fixedSymbolForPyramid[reelIndex][rowIndex]._dug_icon_id = _iconId;
            this.$fixedSymbolForPyramid[reelIndex][rowIndex]._reelIndex = reelIndex;
            this.$fixedSymbolForPyramid[reelIndex][rowIndex]._rowIndex = rowIndex;
            this.setIconTextForPyramid(this.$fixedSymbolForPyramid[reelIndex][rowIndex], _iconId);
        }
    }
    this.updateStaySymbolForPyramid();
}

/*
 * 피라미드 아이콘 텍스트
 */
setIconTextForPyramid(_icon, _iconId, random = false, on = false)
{
    if (_icon == undefined || _iconId != 203) {
        return;
    }
    if (_icon.$attach && _icon.$attach.parent) {
        _icon.$attach.removeLinkSpine();
        _icon.$attach.parent.removeChild(_icon.$attach);
        _icon.$attach.destroy({children : true});
        _icon.$attach = null;
    }
    let isMultipot = false;
    let reelIndex = _icon._reelIndex;
    let rowIndex = _icon._rowIndex;
    let fixPay = 0;
    let type = 0;
    if (random) {
        let pay = [ 500, 300, 150, 100, 50, 30, 20, 15, 12, 10, 5, 3, 2, 1 ];
        let _pay = pay[CMiscUtils.getRandomIntInclusive(0, pay.length - 1)];
        if (_pay >= 50) {
            isMultipot = true;
        }
        fixPay = this._avgTotalBet * _pay / 10;
        type = this.getMultipotForSuper(_pay);
    }
    else {
        fixPay = this._avgTotalBet * this.fixedPayoutForSuper[reelIndex][rowIndex] / 10;
        if (this.fixedPayoutForSuper[reelIndex][rowIndex] > 30) {
            isMultipot = true;
        }
        type = this.getMultipotForSuper(this.fixedPayoutForSuper[reelIndex][rowIndex]);
    }
    let spineNameBox = "s346_icon_pay_box";
    if (on) {
        spineNameBox += "_on";
    }
    let payBox = this.$spineContainer.borrow(spineNameBox);
    if (!isMultipot) {
        let _text = payBox.addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
        _text.text = CMiscUtils.MakeUnitString(fixPay);
        _text.setAutoAlign();
        _icon.addChild(payBox);
        _icon.$attach = payBox;
    }
    else {
        let spineNameMulti = "s346_icon_bonus_a_" + this.multipots[type];
        if (on) {
            spineNameMulti += "_on";
        }
        let _multipot = this.$spineContainer.borrow(spineNameMulti);
        _multipot.setAnimation("on");
        payBox.addLinkSpine(_multipot, "num2");
        _icon.addChild(payBox);
        _icon.$attach = payBox;
    }
}

superFreespinStartAnimationForPyramid()
{
    co(function * () {
        try {
            this.playEffectSound("snd_1st_reel_a");
            let $slotReelWrapper = this.allReels[this.spinType][0];
            let _resultIcons = $slotReelWrapper.getResultIcons();
            for (let rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
                let spineName = "s346_icon_bonus_a_high";
                this.$fixedSymbolForPyramid[0][rowIndex] = this.$spineContainer.borrow(spineName);
                this.$slot.layer_reel_frame_pyramid.addChild(this.$fixedSymbolForPyramid[0][rowIndex]);
                let $reel = this._getTypeReel(0, CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID);
                this.$fixedSymbolForPyramid[0][rowIndex].adjustPosition($reel, rowIndex, 1);
                this.$fixedSymbolForPyramid[0][rowIndex]._dug_icon_id = 203;
                this.$fixedSymbolForPyramid[0][rowIndex]._reelIndex = 0;
                this.$fixedSymbolForPyramid[0][rowIndex]._rowIndex = rowIndex;
                this.$fixedSymbolForPyramid[0][rowIndex].visible = false;
            }
            for (let rowIndex = _resultIcons.length - 1; rowIndex >= 0; rowIndex--) {
                let $icon = _resultIcons[rowIndex];
                $icon.visible = false;
                this.$fixedSymbolForPyramid[0][rowIndex].visible = true;
                this.$fixedSymbolForPyramid[0][rowIndex].setAnimation("appear");
                yield wait(150);
                if (this._is_destroy) {
                    return;
                }
            }
            yield wait(1016);
            if (this._is_destroy) {
                return;
            }
            for (let index = 0; index < this.$fixedSymbolForPyramid[0].length; index++) {
                this.$fixedSymbolForPyramid[0][index].setAnimation("focus");
                this.setIconTextForPyramid(this.$fixedSymbolForPyramid[0][index], 203);
                if (this.$fixedSymbolForPyramid[0][index].$attach) {
                    this.$fixedSymbolForPyramid[0][index].$attach.setAnimation("focus");
                }
            }
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

removeFixedSymbolForPyramid()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            if (this.$fixedSymbolForPyramid[reelIndex][rowIndex] && this.$fixedSymbolForPyramid[reelIndex][rowIndex].parent) {
                this.$fixedSymbolForPyramid[reelIndex][rowIndex].parent.removeChild(this.$fixedSymbolForPyramid[reelIndex][rowIndex]);
                this.$fixedSymbolForPyramid[reelIndex][rowIndex].destroy({children : true});
                this.$fixedSymbolForPyramid[reelIndex][rowIndex] = null;
            }
        }
    }
}

replaceIconLowForPyramid()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let icons = $slotReelWrapper.getResultIcons();
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            $slotReelWrapper.replaceResultIconById(rowIndex, CMiscUtils.getRandomIntInclusive(10004, 10011));
        }
    }
}

/**
 * @protected
 * 모든 히트 심볼 포지션 조회
 */
_findAllHitIconsForPyramid()
{
    let _hitIcons = [];
    if (this._currentResult && this._currentResult.spinResult) {
        let length = this._currentResult.spinResult.payout.lines ? this._currentResult.spinResult.payout.lines.length : 0;
        for (let lineIndex = 0; lineIndex < length; lineIndex++) {
            let _hitline = this._currentResult.spinResult.payout.lines[lineIndex];

            for (let posIndex = 0, posLen = _hitline.position.length; posIndex < posLen; posIndex++) {
                let _pos = _hitline.position[posIndex];
                let _reelIndex = _pos[0];
                let _rowIndex = _pos[1];

                let $icon = this.$fixedSymbolForPyramid[_reelIndex][_rowIndex];
                if (!ArrayUtils.contains(_hitIcons, $icon)) {
                    _hitIcons.push($icon);
                }
            }
        }
    }
    return _hitIcons;
}

isPyramidScatter(_iconId)
{
    return _iconId == 203 || _iconId == 210;
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 아틀란티스 함수
////////////////////////////////////////////////////////////////////////////////////////////
//싱글릴(뱀파이어)
_createSlotReelWrapperForAtlantis(instance, options)
{
    options.onBounceStopSpin = this._onBounceStopSpinForAtlantis.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    let randomStart = CMiscUtils.getRandomIntInclusive(0, options.reelArray.length - 1);
    options.startPosition = randomStart;
    options.spineContainer = this.$spineContainer;
    options.findIndexFunc = this.findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureAtlantisReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
_onBounceStopSpinForAtlantis($slotReelWrapper)
{
    const reelIndex = $slotReelWrapper.reelIndex;
    // 릴 스핀 종료 재생
    let _resultIcons = $slotReelWrapper.getResultIcons();
    let scatterAppear = false;
    let size = reelIndex == 0 ? this._reelWindowHeight : 1;
    for (let rowIndex = 0; rowIndex < size; rowIndex++) {
        let $icon = _resultIcons[rowIndex];
        let _iconId = $icon._dug_icon_id;
        if (!this.isAtlantisScatter(_iconId)) {
            continue;
        }
        $icon.visible = false;
        let spineName = "s346_bonus_b_icon_wild";
        let action = "appear";
        this.$fixedSymbolForAtlantis[reelIndex][rowIndex] = this.$spineContainer.borrow(spineName);
        this.$slot.layer_reel_frame_atlantis.addChild(this.$fixedSymbolForAtlantis[reelIndex][rowIndex]);
        this.$fixedSymbolForAtlantis[reelIndex][rowIndex].setAnimation(action);
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
        this.$fixedSymbolForAtlantis[reelIndex][rowIndex].adjustPosition($reel, rowIndex, 1);
        this.$fixedSymbolForAtlantis[reelIndex][rowIndex]._dug_icon_id = _iconId;
        this.$fixedSymbolForAtlantis[reelIndex][rowIndex]._reelIndex = reelIndex;
        this.$fixedSymbolForAtlantis[reelIndex][rowIndex]._rowIndex = rowIndex;
        if (_iconId == 300) {
            scatterAppear = true;
            this.scatterAppearForAtlantis = true;
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex].multiple = 0;
            let spine = this.$spineContainer.borrow("s346_icon_bonus_b_multiple");
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex].addAsyncLinkSpine(spine, "num");
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex].$attach = spine;
            spine.visible = false;
        }
    }
    if (scatterAppear) {
        this.playEffectSound("snd_multiple_appear");
    }
}

atlantisProc()
{
    this.getHitPaylinesForAtlantis();
    if (this.paylineInfoForAtlantis.length > 0 && this.scatterAppearForAtlantis) {
        for (let index = 0; index < this.paylineInfoForAtlantis.length; index++) {
            let paylineIndex = this.paylineInfoForAtlantis[index];
            let payline = this.paylinesForAtlantis[paylineIndex];
            for (let i = 0; i < 4; i++) {
                if (i == 0) {
                    this.playEffectSpine(
                        "s346_icon_bonus_b_hit_box",
                        this.$fixedSymbolForAtlantis[0][payline[i]],
                        "hit");
                }
                else {
                    this.playEffectSpine(
                        "s346_icon_bonus_b_hit_box",
                        this.$fixedSymbolForAtlantis[(payline[i] + 1) + (9 * (i - 1))][0],
                        "hit");
                }
            }
            if (!this.$payHitAniForAtlantis[paylineIndex]) {
                this.$payHitAniForAtlantis[paylineIndex] = this.$spineContainer.borrow("s346_ui_bonus_b_pay_hit");
                this.$payBoxNumForAtlantis.addAsyncLinkSpine(this.$payHitAniForAtlantis[paylineIndex], "num_" + (paylineIndex + 1));
                this.$payHitAniForAtlantis[paylineIndex].setAnimation("hit");
                this.$payHitAniForAtlantis[paylineIndex].addAnimation("loop", true);
            }
        }
        this.playEffectSound("snd_payline_b1");
        this.$spinProcessQueue.next(1000);
    }
    else {
        this.$spinProcessQueue.next(0);
    }
}

atlantisProc1()
{
    if (this.scatterAppearForAtlantis) {
        co(function * () {
            try {
                this.playEffectSound("snd_multiple_hit");
                for (let reelIndex = 1; reelIndex < this.atlantisReelWidth; reelIndex++) {
                    for (let rowIndex = 0; rowIndex < this.$fixedSymbolForAtlantis[reelIndex].length; rowIndex++) {
                        if (this.$fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                            let $icon = this.$fixedSymbolForAtlantis[reelIndex][rowIndex];
                            if ($icon.multiple == 0) {
                                // let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                                $icon.setAnimation("lock");

                                // $icon.parent.addChild($icon);
                                // $icon.adjustPosition($reel, rowIndex, 1);
                            }
                            // this.setIconTextForPyramid($icon, $icon._dug_icon_id);
                        }
                    }
                }
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this));
        co(function * () {
            try {
                yield wait(1100);
                if (this._is_destroy) {
                    return;
                }
                if (this.scatterAppearForAtlantis && this.$staySpine) {
                    this.$staySpine.setAnimation("hit");
                    this.$staySpine.addAnimation("loop", true);
                }
                let max = 0;
                for (let reelIndex = 1; reelIndex < this.atlantisReelWidth; reelIndex++) {
                    for (let rowIndex = 0; rowIndex < this.$fixedSymbolForAtlantis[reelIndex].length; rowIndex++) {
                        if (this.$fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                            let $icon = this.$fixedSymbolForAtlantis[reelIndex][rowIndex];
                            // let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                            $icon.multiple += 1;
                            if ($icon.multiple > max) {
                                max = $icon.multiple;
                            }
                            let action = "multiple_1";
                            if (reelIndex > 9 && reelIndex < 19) {
                                action = "multiple_2";
                            }
                            else if (reelIndex > 18) {
                                action = "multiple_3";
                            }
                            if ($icon.multiple > 1) {
                                let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                                $icon.setAnimation(action);
                                $icon.parent.addChild($icon);
                                $icon.adjustPosition($reel, rowIndex, 1);
                            }
                            if ($icon.$attach) {
                                $icon.$attach.setAnimation($icon.multiple + "x");
                                $icon.$attach.visible = true;
                            }
                            // $icon.parent.addChild($icon);
                            // $icon.adjustPosition($reel, rowIndex, 1);
                        }
                        // this.setIconTextForPyramid($icon, $icon._dug_icon_id);
                    }
                }
                if (max > 6) {
                    max = 6;
                }
                if (max >= 2) {
                    this.playEffectSound("snd_multiple_pump_b" + (max - 1));
                }
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this));
        this.$spinProcessQueue.next(3000);
    }
    else {
        this.$spinProcessQueue.next(0);
    }
}

atlantisProc2()
{
    if (this.paylineInfoForAtlantis.length > 0 && this.scatterAppearForAtlantis) {
        this.playEffectSound("snd_payline_b2");
        for (let index = 0; index < this.paylineInfoForAtlantis.length; index++) {
            let paylineIndex = this.paylineInfoForAtlantis[index];
            let payline = this.paylinesForAtlantis[paylineIndex];
            this.playEffectSpine(
                "s346_ui_pay_line_" + (paylineIndex + 1),
                this.$slot.eff_container,
                "hit");
            let delay = 166;
            for (let i = 1; i < 4; i++) {
                co(function * () {
                    try {
                        yield wait(delay);
                        if (this._is_destroy) {
                            return;
                        }
                        this.$fixedSymbolForAtlantis[(payline[i] + 1) + (9 * (i - 1))][0].setAnimation("payline");
                    }
                    catch (error) {
                        DUG.log(error);
                    }
                }.bind(this, delay));
                delay += 166;
            }
        }
        this.$spinProcessQueue.next(566);
    }
    else {
        this.$spinProcessQueue.next(0);
    }
}

atlantisProc3()
{
    if (this.paylineInfoForAtlantis.length > 0 && this.scatterAppearForAtlantis) {
        for (let index = 0; index < this.respinAwardForAtlantis.length; index++) {
            let pay = this._avgTotalBet * this.respinAwardForAtlantis[index] / 10;
            if (pay == 0) {
                continue;
            }
            if (this.$payBoxTextForAtlantis[index]) {
                let _duration = 1;
                let _value = {value : 0, init : this.linePayForAtlantis[index], result : (Math.floor(pay)), options : options};
                TweenLite.to(_value, _duration, {
                    value : 1,
                    onUpdate : this.onUpdateLinePayout.bind(this),
                    onUpdateParams : [ _value, index ],
                });
                this.linePayForAtlantis[index] = Math.floor(pay);
            }
            else {
                this.$payBoxTextForAtlantis[index] = this.$payBoxNumForAtlantis.addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
                this.$payBoxTextForAtlantis[index].scale.x = 0.5;
                this.$payBoxTextForAtlantis[index].scale.y = -1 * 0.5;
                this.$payBoxTextForAtlantisBig[index] = this.$payBoxNumForAtlantisBig.addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, this.eventListener);
                this.$payBoxTextForAtlantisBig[index].scale.x = 0.5;
                this.$payBoxTextForAtlantisBig[index].scale.y = -1 * 0.5;
                this.$payBoxTextForAtlantisBig[index].visible = false;
                // 금액 업데이트
                let _duration = 1;
                let _value = {value : 0, init : 0, result : (Math.floor(pay)), options : options};
                TweenLite.to(_value, _duration, {
                    value : 1,
                    onUpdate : this.onUpdateLinePayout.bind(this),
                    onUpdateParams : [ _value, index ],
                });
                this.linePayForAtlantis[index] = Math.floor(pay);
            }
        }
        this.$spinProcessQueue.next(2000);
    }
    else {
        this.$spinProcessQueue.next(0);
    }
}

atlantisProc4()
{
    this.updateStaySymbolForAtlantis();
    this.$spinProcessQueue.next(100);
}

atlantisPayoutProc()
{
    if (this.$payoutSpine) {
        this.stopBGMSound();
        this.playEffectSound("snd_hit_pot_b");
        this.$payoutSpine.setAnimation("in") this.$payoutSpine.addAnimation("loop", true)
    }
    this.$spinProcessQueue.next(2000);
}

atlantisPayoutProc1()
{
    this.removeHitAniForAtlantis();
    if (this.$payBoxNumForAtlantis) {
        this.$slot.eff_container2.addChild(this.$payBoxNumForAtlantis);
        this.$payBoxNumForAtlantis.setAnimation("move");
        this.$slot.eff_container2.addChild(this.$payBoxNumForAtlantisBig);
        this.$payBoxNumForAtlantisBig.setAnimation("move");
    }
    if (this.$staySpine) {
        this.$slot.eff_container3.addChild(this.$staySpine);
        this.$payBoxNumForAtlantisBig.setAnimation("loop", true);
    }
    this.$spinProcessQueue.next(2000);
}

atlantisPayoutProc2()
{
    let count = this.payoutCount();
    let _duration = count * 0.5 + 0.7;
    let _totalAward = this.totalAward();
    this.$payoutTextForAtlantis = this.$payoutSpine.addBitmapText("num", {size : 74, font : "s346_num_congratulations", align : "center", alignY : "center"}, this.eventListener);
    this.$payoutTextForAtlantis.text = "0";
    this.$payoutTextForAtlantis.setAutoAlign();
    this.setOnlyBonusPotPay(true);
    co(function * () {
        try {
            yield wait(300);
            if (this._is_destroy) {
                return;
            }
            let kind = this.getPayoutBeforeEffectKind(_totalAward / this._avgTotalBet);
            if (kind != = 'none') {
                this.$bottomUI.playPayoutWinEffect(kind);
                this.playPayoutBeforeEffectSound(kind);
            }
            this.stopBGMSound();
            this.playBGMSound("bgm_payout_b1");
            this.payoutType = "b1";
            this.$payoutTween = CMiscUtils.playCoinAnimationABB({
                target : this.$payoutTextForAtlantis,
                duration : _duration,
                align : "center",
                start : 0,
                end : _totalAward,
                prefix : "$"
            });
            this.playPayoutIncrement({
                delay : 2000,
                duration : _duration,
                init : 0,
                result : _totalAward,
                payoutType : 'update_bonuspot'
            });
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this, _duration, _totalAward));

    co(function * () {
        try {
            for (let index = 0; index < this.respinAwardForAtlantis.length; index++) {
                if (this.$payBoxTextForAtlantis[index]) {
                    let position = this.getSlotLocalPosision(this.$payBoxNumForAtlantis, "num_" + (index + 1));
                    this.playEffectSound("snd_pot_collect");
                    TweenMax.to(this.$payBoxTextForAtlantis[index], 0.3,
                                {
                                    ease : Power0.easeOut,
                                    x : -position[0],
                                    y : -position[1]
                                });
                    TweenMax.to(this.$payBoxTextForAtlantisBig[index], 0.3,
                                {
                                    ease : Power0.easeOut,
                                    x : -position[0],
                                    y : -position[1]
                                });
                    co(function * () {
                        try {
                            yield wait(300);
                            if (this._is_destroy) {
                                return;
                            }
                            this.$payBoxTextForAtlantis[index].visible = false;
                            this.$payBoxTextForAtlantisBig[index].visible = false;
                            this.$payoutSpine.setAnimation("hit");
                        }
                        catch (error) {
                            DUG.log(error);
                        }
                    }.bind(this, index));
                    yield wait(500);
                    if (this._is_destroy) {
                        return;
                    }
                }
            }
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

atlantisPayoutProc3()
{
    this.removePayBoxForAtlantis();
    this.$payoutTween.kill();
    // this.$payoutSpine.setAnimation("loop", true);
    this.$spinProcessQueue.next(0);
}

payoutCount()
{
    let count = 0;
    for (let index = 0; index < this.respinAwardForAtlantis.length; index++) {
        if (this.respinAwardForAtlantis[index] > 0) {
            count++;
        }
    }
    return count;
}

onUpdateLinePayout(value, index)
{
    let _totalAward = Math.floor(value.init + value.value * (value.result - value.init));
    if (_totalAward >= this._avgTotalBet * 10) {
        this.$payBoxTextForAtlantis[index].visible = false;
        this.$payBoxTextForAtlantisBig[index].visible = true;
    }
    this.$payBoxTextForAtlantis[index].text = CMiscUtils.abbreviatesNumberOption({num : _totalAward, fractionDigits : 1});
    this.$payBoxTextForAtlantis[index].setAutoAlign();
    this.$payBoxTextForAtlantisBig[index].text = CMiscUtils.abbreviatesNumberOption({num : _totalAward, fractionDigits : 1});
    this.$payBoxTextForAtlantisBig[index].setAutoAlign();

    this.$payBoxTextForAtlantis[index].y -= 5;
    this.$payBoxTextForAtlantisBig[index].y -= 5;
}

updateStaySymbolForAtlantis()
{
    for (let reelIndex = 0; reelIndex < this.atlantisReelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this.$fixedSymbolForAtlantis[reelIndex].length; rowIndex++) {
            if (this.$fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                let $icon = this.$fixedSymbolForAtlantis[reelIndex][rowIndex];
                $icon.setAnimation("loop", true);
                $icon.parent.addChild($icon);
                $icon.adjustPosition($reel, rowIndex, 1);
                // this.setIconTextForPyramid($icon, $icon._dug_icon_id);
            }
        }
    }
}
/*
 * 아틀란티스 아이콘 텍스트
 */
setIconTextForAtlantis(_icon, _iconId, random = false, on = false)
{
    if (_icon == undefined || _iconId < 3000) {
        return;
    }
    let fixPay = 0;
    _iconId = _iconId % 3000;
    let pay = [ 5, 6, 7, 8, 9, 10, 20, 30, 40, 50 ];
    fixPay = this._avgTotalBet * pay[_iconId] / 10;
    let _text = _icon.addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
    _text.text = CMiscUtils.abbreviatesNumberOption({num : fixPay, fractionDigits : 1});
    _text.setAutoAlign();
    _icon.addAsyncLinkSpine(_text);
}

getHitPaylinesForAtlantis()
{
    this.paylineInfoForAtlantis = [];
    for (let index = 0; index < this.paylinesForAtlantis.length; index++) {
        let count = 0;
        for (let reelIndex = 0; reelIndex < this.paylinesForAtlantis[index].length; reelIndex++) {
            if (this.fixedPlaylineForSuper[reelIndex][this.paylinesForAtlantis[index][reelIndex]] > 0) {
                count++;
            }
            else {
                break;
            }
        }
        if (count > 3) {
            this.paylineInfoForAtlantis.push(index);
        }
    }
}

removeFixedSymbolForAtlantis()
{
    for (let reelIndex = 0; reelIndex < this.atlantisReelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this.$fixedSymbolForAtlantis[reelIndex].length; rowIndex++) {
            if (this.$fixedSymbolForAtlantis[reelIndex][rowIndex]) {
                this.$fixedSymbolForAtlantis[reelIndex][rowIndex].parent.removeChild(this.$fixedSymbolForAtlantis[reelIndex][rowIndex]);
                this.$fixedSymbolForAtlantis[reelIndex][rowIndex].destroy({children : true});
                this.$fixedSymbolForAtlantis[reelIndex][rowIndex] = null;
            }
        }
    }
}

removePayBoxForAtlantis()
{
    for (let index = 0; index < this.$payBoxTextForAtlantis.length; index++) {
        if (this.$payBoxTextForAtlantis[index]) {
            this.$payBoxTextForAtlantis[index].parent.removeChild(this.$payBoxTextForAtlantis[index]);
            this.$payBoxTextForAtlantis[index].destroy({children : true});
            this.$payBoxTextForAtlantis[index] = null;
        }
    }
    for (let index = 0; index < this.$payBoxTextForAtlantisBig.length; index++) {
        if (this.$payBoxTextForAtlantisBig[index]) {
            this.$payBoxTextForAtlantisBig[index].parent.removeChild(this.$payBoxTextForAtlantisBig[index]);
            this.$payBoxTextForAtlantisBig[index].destroy({children : true});
            this.$payBoxTextForAtlantisBig[index] = null;
        }
    }
    if (this.$payBoxNumForAtlantis) {
        this.$payBoxNumForAtlantis.parent.removeChild(this.$payBoxNumForAtlantis);
        this.$payBoxNumForAtlantis.destroy({children : true});
        this.$payBoxNumForAtlantis = null;
    }
    if (this.$payBoxNumForAtlantisBig) {
        this.$payBoxNumForAtlantisBig.parent.removeChild(this.$payBoxNumForAtlantisBig);
        this.$payBoxNumForAtlantisBig.destroy({children : true});
        this.$payBoxNumForAtlantisBig = null;
    }
    this.removeHitAniForAtlantis();
}

removeHitAniForAtlantis()
{
    for (let index = 0; index < this.$payHitAniForAtlantis.length; index++) {
        if (this.$payHitAniForAtlantis[index]) {
            this.$payHitAniForAtlantis[index].parent.removeChild(this.$payHitAniForAtlantis[index]);
            this.$payHitAniForAtlantis[index].destroy({children : true});
            this.$payHitAniForAtlantis[index] = null;
        }
    }
}

removePayoutSpine()
{
    if (this.$payoutSpine) {
        this.$payoutSpine.parent.removeChild(this.$payoutSpine);
        this.$payoutSpine.destroy({children : true});
        this.$payoutSpine = null;
    }
}

superFreespinStartAnimationForAtlantis()
{
    this.getFeaturedInfo();

    this.$staySpine.setAnimation("in");
    this.$staySpine.addAnimation("side_in");
    this.$staySpine.addAnimation("loop", true);

    co(function * () {
        try {
            this.playEffectSound("snd_1st_reel_b");
            yield wait(833);
            if (this._is_destroy) {
                return;
            }
            let $slotReelWrapper = this.allReels[this.spinType][0];
            let _resultIcons = $slotReelWrapper.getResultIcons();
            for (let rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
                let spineName = "s346_icon_bonus_b_payout";
                this.$fixedSymbolForAtlantis[0][rowIndex] = this.$spineContainer.borrow(spineName);
                this.$slot.layer_reel_frame_atlantis.addChild(this.$fixedSymbolForAtlantis[0][rowIndex]);
                let $reel = this._getTypeReel(0, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
                this.$fixedSymbolForAtlantis[0][rowIndex].adjustPosition($reel, rowIndex, 1);
                this.$fixedSymbolForAtlantis[0][rowIndex]._dug_icon_id = this.fixedPlaylineForSuper[0][rowIndex];
                this.$fixedSymbolForAtlantis[0][rowIndex]._reelIndex = 0;
                this.$fixedSymbolForAtlantis[0][rowIndex]._rowIndex = rowIndex;
                this.$fixedSymbolForAtlantis[0][rowIndex].setAnimation("appear");
                let $icon = _resultIcons[rowIndex];
                $icon.visible = false;
                this.setIconTextForAtlantis(this.$fixedSymbolForAtlantis[0][rowIndex], this.fixedPlaylineForSuper[0][rowIndex]);
                yield wait(90);
                if (this._is_destroy) {
                    return;
                }
            }
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

recoveryProcForAtlantis()
{
    this.getFeaturedInfo();

    this.$payBoxNumForAtlantis = this.$spineContainer.borrow("s346_ui_bonus_b_pay");
    this.$slot.eff_container3.addChild(this.$payBoxNumForAtlantis);
    this.$payBoxNumForAtlantisBig = this.$spineContainer.borrow("s346_ui_bonus_b_pay");
    this.$slot.eff_container3.addChild(this.$payBoxNumForAtlantisBig);
    this.$payBoxTextForAtlantis = this.create1DArray(this.paylinesForAtlantis.length, null);
    this.$payBoxTextForAtlantisBig = this.create1DArray(this.paylinesForAtlantis.length, null);
    this.$payHitAniForAtlantis = this.create1DArray(this.paylinesForAtlantis.length, null);
    this.linePayForAtlantis = this.create1DArray(this.paylinesForAtlantis.length, 0);

    this.$staySpine = this.$spineContainer.borrow("s346_ui_bonus_b_pig");
    this.$staySpine.setAnimation("loop", true);
    this.$slot.eff_container2.addChild(this.$staySpine);
    this.playPayoutSpineForAtlantis();
    this.replaceIconLowForAtlantis();
    for (let reelIndex = 0; reelIndex < this.atlantisReelWidth; reelIndex++) {
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS);
        for (let rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
            let $icon = _resultIcons[rowIndex];
            let _changeReelIndex = reelIndex == 0 ? 0 : Math.floor((reelIndex - 1) / 9) + 1;
            let _changerowIndex = reelIndex == 0 ? rowIndex : (reelIndex - 1) % 9 let _iconId = this.fixedPlaylineForSuper[_changeReelIndex][_changerowIndex];
            // 1->1~9
            // 2->10~18
            // 3->19~27
            if (!this.isAtlantisScatter(_iconId)) {
                continue;
            }
            $icon.visible = false;
            let spineName = "s346_bonus_b_icon_wild";
            if (_iconId >= 3000) {
                spineName = "s346_icon_bonus_b_payout";
            }
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex] = this.$spineContainer.borrow(spineName);
            this.$slot.layer_reel_frame_atlantis.addChild(this.$fixedSymbolForAtlantis[reelIndex][rowIndex]);
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex].adjustPosition($reel, rowIndex, 1);
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex]._dug_icon_id = _iconId;
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex]._reelIndex = reelIndex;
            this.$fixedSymbolForAtlantis[reelIndex][rowIndex]._rowIndex = rowIndex;
            if (_iconId >= 3000) {
                this.setIconTextForAtlantis(this.$fixedSymbolForAtlantis[0][rowIndex], this.fixedPlaylineForSuper[_changeReelIndex][_changerowIndex]);
            }
            else {
                this.$fixedSymbolForAtlantis[reelIndex][rowIndex].multiple = _iconId % 300 == 0 ? 1 : _iconId % 300;
                let spine = this.$spineContainer.borrow("s346_icon_bonus_b_multiple");
                this.$fixedSymbolForAtlantis[reelIndex][rowIndex].addAsyncLinkSpine(spine, "num");
                this.$fixedSymbolForAtlantis[reelIndex][rowIndex].$attach = spine;
                if (this.$fixedSymbolForAtlantis[reelIndex][rowIndex].$attach) {
                    this.$fixedSymbolForAtlantis[reelIndex][rowIndex].$attach.setAnimation(this.$fixedSymbolForAtlantis[reelIndex][rowIndex].multiple + "x");
                    this.$fixedSymbolForAtlantis[reelIndex][rowIndex].$attach.visible = true;
                }
            }
        }
    }
    for (let index = 0; index < this.respinAwardForAtlantis.length; index++) {
        let pay = this._avgTotalBet * this.respinAwardForAtlantis[index] / 10;
        if (pay == 0) {
            continue;
        }
        this.$payHitAniForAtlantis[index] = this.$spineContainer.borrow("s346_ui_bonus_b_pay_hit");
        this.$payBoxNumForAtlantis.addAsyncLinkSpine(this.$payHitAniForAtlantis[index], "num_" + (index + 1));
        this.$payHitAniForAtlantis[index].setAnimation("loop", true);
        this.$payBoxTextForAtlantis[index] = this.$payBoxNumForAtlantis.addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
        this.$payBoxTextForAtlantis[index].scale.x = 0.5;
        this.$payBoxTextForAtlantis[index].scale.y = -1 * 0.5;
        this.$payBoxTextForAtlantisBig[index] = this.$payBoxNumForAtlantisBig.addBitmapText("num_" + (index + 1), {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, this.eventListener);
        this.$payBoxTextForAtlantisBig[index].scale.x = 0.5;
        this.$payBoxTextForAtlantisBig[index].scale.y = -1 * 0.5;
        this.$payBoxTextForAtlantisBig[index].visible = false;
        if (pay >= this._avgTotalBet * 10) {
            this.$payBoxTextForAtlantis[index].visible = false;
            this.$payBoxTextForAtlantisBig[index].visible = true;
        }
        this.$payBoxTextForAtlantis[index].text = CMiscUtils.abbreviatesNumberOption({num : pay, fractionDigits : 1});
        this.$payBoxTextForAtlantis[index].setAutoAlign();
        this.$payBoxTextForAtlantisBig[index].text = CMiscUtils.abbreviatesNumberOption({num : pay, fractionDigits : 1});
        this.$payBoxTextForAtlantisBig[index].setAutoAlign();
        this.$payBoxTextForAtlantis[index].y -= 5;
        this.$payBoxTextForAtlantisBig[index].y -= 5;
        this.linePayForAtlantis[index] = Math.floor(pay);
    }
    this.updateStaySymbolForAtlantis();
}

replaceIconLowForAtlantis()
{
    for (let reelIndex = 0; reelIndex < this.atlantisReelWidth; reelIndex++) {
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        for (let rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
            $slotReelWrapper.replaceResultIconById(rowIndex, CMiscUtils.getRandomIntInclusive(10004, 10011));
        }
    }
}

isAtlantisScatter(iconId)
{
    return (iconId >= 300 && iconId <= 400) || (iconId >= 3000 && iconId <= 3100);
}
////////////////////////////////////////////////////////////////////////////////////////////
// 슈퍼프리스핀 오리엔탈 함수
////////////////////////////////////////////////////////////////////////////////////////////
//일반릴(카멜레온)
_createSlotReelWrapperForOriental(instance, options)
{
    options.onBounceStopSpin = this._onBounceStopSpinForOriental.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    let randomStart = CMiscUtils.getRandomIntInclusive(0, options.reelArray.length - 1);
    options.startPosition = randomStart;
    options.spineContainer = this.$spineContainer;
    options.findIndexFunc = this.findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureOrientalReelWrapper(instance, options);
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
_onBounceStopSpinForOriental($slotReelWrapper)
{
    const reelIndex = $slotReelWrapper.reelIndex;
    // 릴 스핀 종료 재생
    let _resultIcons = $slotReelWrapper.getResultIcons();
    let scatterAppear = false;
    for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
        let $icon = _resultIcons[rowIndex];
        let _iconId = $icon._dug_icon_id;
        if (!this.isOrientalScatter(_iconId)) {
            continue;
        }
        if (_iconId == 420) {
            $icon.visible = false;
        }
        else {
            continue;
        }
        scatterAppear = true;
        let spineName = "s346_icon_bonus_c_scatter";
        let action = "appear";
        this.$fixedSymbolForOriental[reelIndex][rowIndex] = this.$spineContainer.borrow(spineName);
        this.$slot.layer_reel_frame_oriental.addChild(this.$fixedSymbolForOriental[reelIndex][rowIndex]);
        this.$fixedSymbolForOriental[reelIndex][rowIndex].setAnimation(action);
        let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);
        this.$fixedSymbolForOriental[reelIndex][rowIndex].adjustPosition($reel, rowIndex, 1);
        this.$fixedSymbolForOriental[reelIndex][rowIndex]._dug_icon_id = _iconId;
        this.$fixedSymbolForOriental[reelIndex][rowIndex]._reelIndex = reelIndex;
        this.$fixedSymbolForOriental[reelIndex][rowIndex]._rowIndex = rowIndex;
    }
    this.playEffectSound("snd_reel_stop_c");
    if (scatterAppear) {
        this.playEffectSound("snd_pig_appear");
    }
}

orientalPayoutProc()
{
    this.setOnlyBonusPotPay(true);
    this.bonuspot_number = this.orientalTotalAward;
    this.orientalCoinCount = 0;
    this.orientalMultiCount = 0;
    this.fixedPayForOriental = this.create2DArray(this._reelWidth, this._reelWindowHeight, 0);
    this.$spinProcessQueue.next(500);
}

orientalPayoutProc1()
{
    this.updateStaySymbolForOriental();
    if (this.orientalCoinCount > 0 || this.orientalMultiCount > 0) {
        let delay = 1166 + (this.orientalCoinCount + this.orientalMultiCount * 200) + 1333 + 500;
        for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
            if (!ArrayUtils.contains(this.hitReelForOriental, reelIndex)) {
                continue;
            }
            let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);
            let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
            let _resultIcons = $slotReelWrapper.getResultIcons();
            for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                let $icon = _resultIcons[rowIndex];
                let iconId = $icon._dug_icon_id;
                if (iconId == 420 && this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
                    this.focusAniForOriental(reelIndex, rowIndex);
                }
            }
        }
        this.focusSoundForOriental();
        this.$spinProcessQueue.next(delay);
    }
    else {
        this.$spinProcessQueue.next(0);
    }
}

orientalPayoutProc2()
{
    if (this.orientalCoinCount > 0) {
        let delay = (this.orientalCoinCount * 533) + 833 + 500;
        for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
            if (!ArrayUtils.contains(this.hitReelForOriental, reelIndex)) {
                continue;
            }
            let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
            let _resultIcons = $slotReelWrapper.getResultIcons();
            for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                let $icon = _resultIcons[rowIndex];
                let iconId = $icon._dug_icon_id;
                if (iconId == 420 && this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
                    this.coinPayoutForOriental(reelIndex, rowIndex);
                }
            }
        }
        this.coinPayoutSoundForOriental();
        this.$spinProcessQueue.next(delay);
    }
    else {
        this.$spinProcessQueue.next(0);
    }
}

orientalPayoutProc3()
{
    if (this.orientalMultiCount > 0) {
        co(function * () {
            try {
                for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
                    if (!ArrayUtils.contains(this.hitReelForOriental, reelIndex)) {
                        continue;
                    }
                    let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
                    let _resultIcons = $slotReelWrapper.getResultIcons();
                    for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                        let $icon = _resultIcons[rowIndex];
                        let iconId = $icon._dug_icon_id;
                        if (iconId == 420 && this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
                            this.$fixedSymbolForOriental[reelIndex][rowIndex].parent.addChild(this.$fixedSymbolForOriental[reelIndex][rowIndex]);
                            let count = 1;
                            for (let index = rowIndex - 1; index >= 0; index--) {
                                let $aIcon = _resultIcons[index];
                                let iconId = $aIcon._dug_icon_id;
                                if (iconId == 440 && this.$fixedSymbolForOriental[reelIndex][index]) {
                                    $aIcon.visible = true;
                                    $aIcon.setAnimation("dim");
                                    this.$fixedSymbolForOriental[reelIndex][index].visible = true;
                                    this.$slot.layer_reel_up.addChild(this.$fixedSymbolForOriental[reelIndex][index]);
                                    this.$fixedSymbolForOriental[reelIndex][index].setAnimation("save_" + count);
                                    let type = this.getMultipotForSuper(this.fixedPayoutForSuper[reelIndex][index]);
                                    this.playEffectSound("snd_" + this.multipots[type]);
                                    if (this.$fixedSymbolForOriental[reelIndex][index].$attach) {
                                        this.$fixedSymbolForOriental[reelIndex][index].$attach.setAnimation("save");
                                        this.$fixedSymbolForOriental[reelIndex][index].$attach.visible = true;
                                    }
                                    this.scatterSaveForOriental(reelIndex, rowIndex, index, true);
                                    this.playEffectSound("snd_pig_text");
                                    yield wait(3666);
                                    if (this._is_destroy) {
                                        return;
                                    }
                                }
                                count++;
                            }
                        }
                    }
                }
                this.$spinProcessQueue.next(500);
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this));
    }
    else {
        this.$spinProcessQueue.next(0);
    }
}

orientalPayoutProc4()
{
    if (this.$bottomUI) {
        this.$bottomUI.playPayoutEffect();
    }
    this.orientalTotalAwardPre = this.orientalTotalAward;
    this.playEffectSound("snd_pig_pay");
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        if (!ArrayUtils.contains(this.hitReelForOriental, reelIndex)) {
            continue;
        }
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            let $icon = _resultIcons[rowIndex];
            let iconId = $icon._dug_icon_id;
            if (iconId == 420 && this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
                this.$fixedSymbolForOriental[reelIndex][rowIndex].setAnimation("hit_in");
                this.$fixedSymbolForOriental[reelIndex][rowIndex].addAnimation("hit_loop", true);
            }
        }
    }
    if (this.$staySpine) {
        this.$staySpine.setAnimation("hit", true);
    }
    this.$spinProcessQueue.next(3000);
}

orientalPayoutProc5()
{
    if (this.$staySpine) {
        this.$staySpine.setAnimation("side_loop", true);
    }
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        if (!ArrayUtils.contains(this.hitReelForOriental, reelIndex)) {
            continue;
        }
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            let $icon = _resultIcons[rowIndex];
            let iconId = $icon._dug_icon_id;
            if (iconId == 420 && this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
                $icon.visible = true;
                $icon.setAnimation("on2");
                let fixPay = this.fixedPayForOriental[reelIndex][rowIndex];
                let _text = null;
                let slot = $icon.getSlotContainer("num");
                if (slot && slot.$text) {
                    slot._dug_init_textfield = false;
                    slot.$text.parent.removeChild(slot.$text);
                    slot.$text.destroy({children : true});
                    delete slot.$text;
                    slot.$text = null;
                }
                if (fixPay >= this._avgTotalBet * 5) {
                    _text = $icon.addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, this.eventListener);
                }
                else {
                    _text = $icon.addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
                }
                _text.scale.x = 0.625;
                _text.scale.y = -1 * 0.625;
                _text.text = CMiscUtils.MakeUnitString(fixPay);
                _text.setAutoAlign();
                $icon.$attach = _text;
            }
        }
    }
    this.removeFixedSymbolForOriental();
    this.$spinProcessQueue.next(10);
}

focusAniForOriental(reelIndex, rowIndex)
{
    co(function * () {
        try {
            let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
            let _resultIcons = $slotReelWrapper.getResultIcons();
            let $reel = this._getTypeReel(reelIndex, CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL);

            if (this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
                this.$fixedSymbolForOriental[reelIndex][rowIndex].setAnimation("focus");
                this.$fixedSymbolForOriental[reelIndex][rowIndex].addAnimation("stay2", true);
                let _text = this.$fixedSymbolForOriental[reelIndex][rowIndex].addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
                _text.text = CMiscUtils.MakeUnitString(0);
                _text.setAutoAlign();
                this.fixedPayForOriental[reelIndex][rowIndex] = 0;
                if (rowIndex > 0) {
                    let $spine = this.$spineContainer.borrow("s346_ui_bonus_c_reel_eff2");
                    $spine.setAnimation("focus_" + rowIndex);
                    this.$slot.layer_reel_frame_oriental.addChild($spine);
                    $spine.adjustPosition($reel, rowIndex, 1);
                    $spine.addAnimationComplete(
                        "focus_" + rowIndex, (entry) = > {
                            co(function * () {
                                try {
                                    yield wait(0);
                                    if (this._is_destroy) {
                                        return;
                                    }
                                    if ($spine) {
                                        $spine.parent.removeChild($spine);
                                    }
                                }
                                catch (error) {
                                    DUG.log(error);
                                }
                            }.bind(this, $spine));
                        });
                }
                yield wait(1166);
                if (this._is_destroy) {
                    return;
                }
            }
            for (let index = rowIndex - 1; index >= 0; index--) {
                let $icon = _resultIcons[index];
                let iconId = $icon._dug_icon_id;
                if (iconId == 430 || iconId == 440) {
                    if (this.$fixedSymbolForOriental[reelIndex][index]) {
                        this.$fixedSymbolForOriental[reelIndex][index].parent.removeChild(this.$fixedSymbolForOriental[reelIndex][index]);
                        this.$fixedSymbolForOriental[reelIndex][index].destroy({children : true});
                        this.$fixedSymbolForOriental[reelIndex][index] = null;
                    }
                    let spineName = "s346_icon_bonus_c_coin";
                    if (iconId == 440) {
                        spineName = "s346_icon_bonus_c_gem";
                    }
                    this.$fixedSymbolForOriental[reelIndex][index] = this.$spineContainer.borrow(spineName);
                    this.$slot.layer_reel_frame_oriental.animation.addChild(this.$fixedSymbolForOriental[reelIndex][index]);
                    this.$fixedSymbolForOriental[reelIndex][index].adjustPosition($reel, index, 1);
                    this.$fixedSymbolForOriental[reelIndex][index]._dug_icon_id = iconId;
                    this.$fixedSymbolForOriental[reelIndex][index]._reelIndex = reelIndex;
                    this.$fixedSymbolForOriental[reelIndex][index]._rowIndex = index;
                    this.setIconTextForOriental(this.$fixedSymbolForOriental[reelIndex][index], iconId);
                    if (this.$fixedSymbolForOriental[reelIndex][index]) {
                        $icon.visible = false;
                        this.$fixedSymbolForOriental[reelIndex][index].setAnimation("focus");
                        this.$fixedSymbolForOriental[reelIndex][index].addAnimation("loop", true);
                        yield wait(200);
                        if (this._is_destroy) {
                            return;
                        }
                    }
                }
            }
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

focusSoundForOriental()
{
    co(function * () {
        try {
            this.playEffectSound("snd_pig_hit");
            yield wait(1166);
            if (this._is_destroy) {
                return;
            }
            this.playEffectSoundWithOption("snd_line_start", "snd_line_start");
            yield wait(200 * (this.orientalCoinCount + this.orientalMultiCount));
            if (this._is_destroy) {
                return;
            }
            this.stopEffectSound("snd_line_start");
            this.playEffectSound("snd_coin_shaking");
            this.playEffectSound("snd_line_end");
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

coinPayoutForOriental(reelIndex, rowIndex)
{
    co(function * () {
        try {
            let count = 1;
            let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
            let _resultIcons = $slotReelWrapper.getResultIcons();
            for (let index = rowIndex - 1; index >= 0; index--) {
                let $icon = _resultIcons[index];
                let iconId = $icon._dug_icon_id;
                if (iconId == 430 && this.$fixedSymbolForOriental[reelIndex][index]) {
                    $icon.visible = true;
                    $icon.setAnimation("dim");
                    this.$slot.layer_reel_up.addChild(this.$fixedSymbolForOriental[reelIndex][index]);
                    this.$fixedSymbolForOriental[reelIndex][index].setAnimation("save_" + count);
                    co(function * () {
                        try {
                            yield wait(833);
                            if (this._is_destroy) {
                                return;
                            }
                            this.$fixedSymbolForOriental[reelIndex][index].visible = false;
                        }
                        catch (error) {
                            DUG.log(error);
                        }
                    }.bind(this));
                    yield wait(533);
                    if (this._is_destroy) {
                        return;
                    }
                    this.scatterSaveForOriental(reelIndex, rowIndex, index, false);
                }
                count++;
            }
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

coinPayoutSoundForOriental()
{
    co(function * () {
        try {
            for (let _count = 0; _count < this.orientalCoinCount; _count++) {
                this.playEffectSound("snd_coin_hit_c" + (_count + 1));
                yield wait(533);
                if (this._is_destroy) {
                    return;
                }
            }
        }
        catch (error) {
            DUG.log(error);
        }
    }.bind(this));
}

scatterSaveForOriental(reelIndex, rowIndex, index, multi)
{
    if (this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
        let action = "save";
        let delay = 300;
        if (multi) {
            action = "save_m";
            delay = 2933;
        }
        this.$fixedSymbolForOriental[reelIndex][rowIndex].setAnimation(action);
        this.$fixedSymbolForOriental[reelIndex][rowIndex].addAnimationComplete(
            action, (entry) = > {
                co(function * () {
                    try {
                        yield wait(0);
                        if (this._is_destroy) {
                            return;
                        }
                        this.$fixedSymbolForOriental[reelIndex][rowIndex].setAnimation("stay2", true);
                    }
                    catch (e) {
                        DUG.log(e);
                    }
                }.bind(this));
            });
        co(function * () {
            try {
                yield wait(delay);
                if (this._is_destroy) {
                    return;
                }
                if (this.$bottomUI) {
                    this.$bottomUI.playPayoutEffect();
                }
                if (this.$fixedSymbolForOriental[reelIndex][rowIndex].$attach) {
                    let slot = this.$fixedSymbolForOriental[reelIndex][rowIndex].getSlotContainer("num");
                    if (slot && slot.$text) {
                        slot._dug_init_textfield = false;
                        slot.$text.parent.removeChild(slot.$text);
                        slot.$text.destroy({children : true});
                        delete slot.$text;
                        slot.$text = null;
                        this.$fixedSymbolForOriental[reelIndex][rowIndex].$attach = null;
                    }
                }
                let fixPay = this._avgTotalBet * this.fixedPayoutForSuper[reelIndex][index] / 10;
                this.orientalTotalAward += fixPay;
                this.bonuspot_number = this.orientalTotalAward;
                this.fixedPayForOriental[reelIndex][rowIndex] += fixPay;
                let _text = null;
                if (this.fixedPayForOriental[reelIndex][rowIndex] >= this._avgTotalBet * 5) {
                    _text = this.$fixedSymbolForOriental[reelIndex][rowIndex].addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_yellow", align : "center", alignY : "center"}, this.eventListener);
                }
                else {
                    _text = this.$fixedSymbolForOriental[reelIndex][rowIndex].addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
                }
                _text.text = CMiscUtils.MakeUnitString(this.fixedPayForOriental[reelIndex][rowIndex]);
                _text.setAutoAlign();
                this.$fixedSymbolForOriental[reelIndex][rowIndex].$attach = _text;
            }
            catch (error) {
                DUG.log(error);
            }
        }.bind(this, delay));
    }
}

setTintForOriental($icon, tint)
{
    let _tint = 0x404C42;
    let alpha = 0.75;
    if (!tint) {
        _tint = 0xFFFFFF;
        alpha = 1;
    }
    $icon.tint = _tint;
    $icon.alpha = alpha;
    if ($icon.$attach) {
        $icon.$attach.tint = _tint;
        $icon.$attach.alpha = alpha;
    }
    let slot = $icon.getSlotContainer("num");
    if (slot && slot.$text) {
        slot.$text.tint = _tint;
        slot.$text.alpha = 1;
    }
}

updateStaySymbolForOriental()
{
    let maxCountCoin = 0;
    let maxCountMulti = 0;
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $slotReelWrapper = this.allReels[this.spinType][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        let reel_inner = $slotReelWrapper.$reel_inner.children;
        let coinCount = 0;
        let multiCount = 0;
        for (let index = 0; index < reel_inner.length; index++) {
            let $icon = reel_inner[index];
            if (!ArrayUtils.contains(_resultIcons, $icon)) {
                this.setTintForOriental($icon, true);
            }
        }
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            let $icon = _resultIcons[rowIndex];
            let iconId = $icon._dug_icon_id;
            if (iconId == 430) {
                coinCount++;
            }
            else if (iconId == 440) {
                multiCount++;
            }
            if (iconId == 420 && this.$fixedSymbolForOriental[reelIndex][rowIndex]) {
                this.$fixedSymbolForOriental[reelIndex][rowIndex].setAnimation("saty", true);
                if (maxCountCoin < coinCount) {
                    maxCountCoin = coinCount;
                }
                if (maxCountMulti < multiCount) {
                    maxCountMulti = multiCount;
                }
                for (let index = rowIndex + 1; index < this._reelWindowHeight; index++) {
                    this.setTintForOriental(_resultIcons[index], true);
                }
            }
            if (multiCount + coinCount == 9) {
                for (let index = 0; index < this._reelWindowHeight; index++) {
                    this.setTintForOriental(_resultIcons[index], true);
                }
            }
        }
    }
    this.orientalCoinCount = maxCountCoin;
    this.orientalMultiCount = maxCountMulti;
}

recoveryProcForOriental()
{
    this.getFeaturedInfo();
    this.orientalTotalAward = this.superTotalAward;
    this.orientalTotalAwardPre = this.orientalTotalAward;
    this.$staySpine = this.$spineContainer.borrow("s346_ui_bonus_c_pig");
    this.$staySpine.setAnimation("side_loop", true);
    this.$slot.eff_container2.addChild(this.$staySpine);
    this.replaceIconLowForOriental();
}

/*
 * 오리엔탈 아이콘 텍스트
 */
setIconTextForOriental(_icon, _iconId, random = false, on = false)
{
    if (_icon == undefined || (_iconId != 430 && _iconId != 440)) {
        return;
    }
    _icon.removeLinkSpine();
    if (_icon.$attach && _icon.$attach.parent) {
        _icon.$attach.parent.removeChild(_icon.$attach);
        _icon.$attach.destroy({children : true});
        _icon.$attach = null;
    }
    let isMultipot = false;
    let reelIndex = _icon._reelIndex;
    let rowIndex = _icon._rowIndex;
    let fixPay = 0;
    let type = 0;
    if (random) {
        if (_iconId == 430) {
            let pay = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40 ];
            let _pay = pay[CMiscUtils.getRandomIntInclusive(0, pay.length - 1)];
            fixPay = this._avgTotalBet * _pay / 10;
        }
        else {
            type = CMiscUtils.getRandomIntInclusive(1, this.multipots.length - 1);
            isMultipot = true;
        }
    }
    else {
        fixPay = this._avgTotalBet * this.fixedPayoutForSuper[reelIndex][rowIndex] / 10;
        if (this.fixedPayoutForSuper[reelIndex][rowIndex] > 40) {
            isMultipot = true;
        }
        type = this.getMultipotForSuper(this.fixedPayoutForSuper[reelIndex][rowIndex]);
    }
    if (!isMultipot) {
        let _text = _icon.addBitmapText("num", {size : 64, font : "s346_num_icon_scatter_white", align : "center", alignY : "center"}, this.eventListener);
        _text.text = CMiscUtils.MakeUnitString(fixPay);
        _text.scale.x = 0.625;
        _text.scale.y = -1 * 0.625;
        _text.setAutoAlign();
    }
    else {
        let spineNameMulti = "s346_icon_bonus_c_" + this.multipots[type];
        if (on) {
            spineNameMulti += "_on";
        }
        let _multipot = this.$spineContainer.borrow(spineNameMulti);
        _multipot.setAnimation("on");
        _icon.addLinkSpine(_multipot, "num");
        _icon.$attach = _multipot;
    }
}

removeFixedSymbolForOriental()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            if (this.$fixedSymbolForOriental[reelIndex][rowIndex] && this.$fixedSymbolForOriental[reelIndex][rowIndex].parent) {
                this.$fixedSymbolForOriental[reelIndex][rowIndex].parent.removeChild(this.$fixedSymbolForOriental[reelIndex][rowIndex]);
                this.$fixedSymbolForOriental[reelIndex][rowIndex].destroy({children : true});
                this.$fixedSymbolForOriental[reelIndex][rowIndex] = null;
            }
        }
    }
}

_setResultIconsAlphaForOriental(alpha)
{
    for (var reelIndex = 0; reelIndex < this.slotReels.length; reelIndex++) {
        let $slotReelWrapper = this.allReels[CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL][reelIndex];
        var _resultIcons = $slotReelWrapper.getResultIcons();
        for (var rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
            var $icon = _resultIcons[rowIndex];
            $icon.alpha = alpha;
        }
    }
}

replaceIconLowForOriental()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $slotReelWrapper = this.allReels[CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL][reelIndex];
        let _resultIcons = $slotReelWrapper.getResultIcons();
        for (let rowIndex = 0; rowIndex < _resultIcons.length; rowIndex++) {
            $slotReelWrapper.replaceResultIconById(rowIndex, 430);
            _resultIcons[rowIndex].setAnimation("on");
        }
    }
}

isOrientalScatter(_iconId)
{
    return _iconId == 420 || _iconId == 430 || _iconId == 440;
}
////////////////////////////////////////////////////////////////////////////////////////////
//기본 디렉션 함수
////////////////////////////////////////////////////////////////////////////////////////////
/**
 * 디렉션 세팅 기본
 */
_setDirections()
{
    this.getFeaturedInfo();
    let spinResult = this._currentResult.spinResult;
    let payout = spinResult.payout;
    let isRealJackpot = this.isRealJackpot;
    let bigwin = (payout.isBigwin&& isRealJackpot == = false);
    let totalAward = this.totalAward();
    if (this.isPick && !this.isSuperFreespin) {
        this._setDirection('_step_Pick_Proc');
    }
    if (this.isSuperFreespin) {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            this._setDirection('_step_Pyramid_Proc');
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            this._setDirection('_step_Atlantis_Proc');
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            this._setDirection('_step_Oriental_Payout');
        }
    }
    if (spinResult.freespin) {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            this._setDirection('_step_Bonus_Proc');
        }
    }
    if (totalAward > 0) {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
            this._setDirection('_step_Payout');
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            this._setDirection('_step_Bonus_Payout');
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            this._setDirection('_step_Pyramid_Payout');
        }
        else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
            this._setDirection('_step_Atlantis_Payout');
        }
    }
    let isJackpot = (payout.isJackpot || this.jackpotObject);
    if (isJackpot || payout.jackpotAward > 0) {
        this._setDirection('_step_Jackpot');
    }
    if (bigwin == = true &&
                    this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS &&
                    this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        this._setDirection('_step_Bigwin');
    }
    if (payout.totalAward > 0) {
        this._setDirection('_step_UpdateUserBalance');
    }
    if (spinResult.freespin && !this.superFreespinStart) {
        if (spinResult.freespin.isStart == = true && spinResult.freespin.remainCount > 0) {
            this._setDirection('_step_BonusIn');
        }
        else if (spinResult.freespin.remainCount == 0) {
            this._setDirection('_step_BonusOut');
        }
    }
    if (this.superFreespinStart) {
        this._setDirection('_step_SuperIn');
    }
    else if (this.superFreespinEnd) {
        this._setDirection('_step_SuperOut');
    }
    if (bigwin == = true &&
                    (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS || this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL)) {
        this._setDirection('_step_Bigwin');
    }
    if (this.pickType > 0 && this.pickStart) {
        this._setDirection('_step_PickIn');
    }
    else if (this.pickEnd && !this.superFreespinStart) {
        this._setDirection('_step_PickOut');
    }
    else if (this.isPick && !this.superFreespin && !this.superFreespinStart) {
        this._setDirection('_step_WaitPick');
    }
    else {
        if (bigwin == false && isRealJackpot == false && payout.totalAward <= 0) {
            this._setDirection("_step_Idle");
        }
    }
    this._setDirection('_step_End');
    if (this.isPick) {
        this.startDirectionProc(100);
    }
}

_addStepFunc()
{
    super._addStepFunc();

    this._directionController.addStepFunction(
        "_step_Payout", () = > {
            let spinResult = this._currentResult.spinResult;
            let payout = spinResult.payout;
            let isJackpot = payout.isJackpot;
            let bigwin = (payout.isBigwin&& isJackpot == = false);
            if ((payout.totalAward > 0 ||
                 payout.lines.length > 0)&& bigwin == false&& isJackpot == = false) {
                this._directionController.makeSmallSteps("onPayoutBeforeEffect", "onPayout");
            }
            else {
                if (bigwin || isJackpot) {
                    this._directionController.makeSmallSteps("onPayoutBeforeEffect", "onPayout");
                }
                else {
                    this._directionController.onNextCombineQueue();
                }
            }
        });
    _directionController->addStepFunc("_step_Bigwin", [=]() {
        _directionController->makeSmallSteps("onBigwinBefore", "onBigwin", "onBigwinEnd", "");
    });
    this._directionController.addStepFunction(
        "_step_BonusIn", () = > {
            this._directionController.makeSmallSteps("onStartBonusGames", "onStartBonusGames1", "onStartBonusGames2");
        });

    this._directionController.addStepFunction(
        "_step_BonusOut", () = > {
            this._directionController.makeSmallSteps("onEndBonusGames", "onEndBonusGames1", "onEndBonusGames2");
        });
    this._directionController.addStepFunction(
        "_step_Bonus_Proc", () = > {
            this._directionController.makeSmallSteps("onDelay", "freespinProc", "freespinProc1");
        });
    this._directionController.addStepFunction(
        "_step_Bonus_Payout", () = > {
            this._directionController.makeSmallSteps("freespinPayoutProc", "onPayoutBeforeEffect", "onDelay", "freespinPayoutProc1", "freespinPayoutProc2");
        });
    this._directionController.addStepFunction(
        "_step_PickIn", () = > {
            this._directionController.makeSmallSteps("onStartPickBonusGames", "onStartPickBonusGames1", "onStartPickBonusGames2", "onStartPickBonusGames3", "onWaitPick");
        });
    this._directionController.addStepFunction(
        "_step_PickOut", () = > {
            this._directionController.makeSmallSteps("onEndPickBonusGames", "onEndPickBonusGames1");
        });
    this._directionController.addStepFunction(
        '_step_WaitPick', () = > {
            this._directionController.makeSmallSteps('onWaitPick');
        });
    this._directionController.addStepFunction(
        "_step_SuperIn", () = > {
            this._directionController.makeSmallSteps("onStartSuperFreespin", "onStartSuperFreespin1", "onStartSuperFreespin2");
        });
    this._directionController.addStepFunction(
        "_step_SuperOut", () = > {
            this._directionController.makeSmallSteps("onEndSuperFreespin", "onEndSuperFreespin1", "onEndSuperFreespin2");
        });
    this._directionController.addStepFunction(
        "_step_Pick_Proc", () = > {
            this._directionController.makeSmallSteps("pickProc");
        });
    this._directionController.addStepFunction(
        "_step_Pyramid_Proc", () = > {
            this._directionController.makeSmallSteps("onDelay", "pyramidProc");
        });
    this._directionController.addStepFunction(
        "_step_Pyramid_Payout", () = > {
            this._directionController.makeSmallSteps("pyramidPayoutProc", "pyramidPayoutProc1", "pyramidPayoutProc2");
        });
    this._directionController.addStepFunction(
        "_step_Oriental_Payout", () = > {
            this._directionController.makeSmallSteps("orientalPayoutProc", "orientalPayoutProc1", "orientalPayoutProc2", "orientalPayoutProc3", "onPayoutBeforeEffect", "onDelay", "orientalPayoutProc4", "orientalPayoutProc5");
        });
    this._directionController.addStepFunction(
        "_step_Atlantis_Proc", () = > {
            this._directionController.makeSmallSteps("onDelay", "atlantisProc", "atlantisProc1", "atlantisProc2", "atlantisProc3", "atlantisProc4");
        });
    this._directionController.addStepFunction(
        "_step_Atlantis_Payout", () = > {
            this._directionController.makeSmallSteps("atlantisPayoutProc", "atlantisPayoutProc1", "atlantisPayoutProc2", "atlantisPayoutProc3");
        });

    this._directionController.addStepFunction(
        "_step_Idle", () = > {
            this._directionController.makeSmallSteps("onIdleSymbol");
        });
}

onDelay()
{
    this.$spinProcessQueue.next(500);
}

/**
 * @protected
 * 페이아웃 처리
 */
onPayout(options)
{
    // 페이라인으로 전체를 보여줌
    this.isClick = true;
    if (this._currentResult.spinResult.payout.lines) {
        this.showHitPaylines();
        this.playHidePaylineProcess();
    }
    this._releaseAnimationIcons();
    // 히트 포지션의 모든 심볼에 히트 연출 발동
    this._playHitAnimation();

    // 페이아웃 이펙트 처리
    let _update_bonuspot = false;
    this._bPayoutSkipping = false;
    let _delay = 2000;
    let _totalAward = this.totalAward();
    let _duration = this.calcNewCoinIncreaseTime(_totalAward);
    this.payoutType = "1";
    this.playEffectSoundWithOption("snd_payout_" + this.payoutType, "snd_payout_" + this.payoutType, true);
    this.changeBGMVolume(0.5);
    if (this.isPick) {
        _delay = 0;
        _duration = this.calcNewCoinIncreaseTimeForPick(_totalAward);
        this.setOnlyBonusPotPay(true);
    }
    super.onPayout({
        delay : _delay,
        duration : _duration,
        totalAward : _totalAward,
        showWin : true,
        payoutType : !_update_bonuspot ? 'update_bonuspot' : ''
    });
}

/**
 * 페이아웃 하단 이펙트 출력
 *  @protected
 */
onPayoutBeforeEffect()
{
    let award = this._currentResult.spinResult.payout.totalAward;
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        award = this.totalAward();
    }
    let multi = award / this._totalBet;
    if (this.isSuperFreespin || this.isPick) {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            award = this.orientalTotalAward - this.orientalTotalAwardPre;
        }
        multi = award / this._avgTotalBet;
    }
    let kind = this.getPayoutBeforeEffectKind(multi);
    if (kind != = 'none') {
        this.$bottomUI.playPayoutWinEffect(kind);
        this.playPayoutBeforeEffectSound(kind);
        this.$spinProcessQueue.next(500);
    }
    else {
        this.$spinProcessQueue.next();
    }
}

/**
 * onPayoutCompleteValue
 */
onPayoutCompleteValue(value)
{
    if (this.spinType != CSlot346PiggyAdventure.SPIN_TYPE.NORMAL) {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
            let _totalAward = value.result;
            this._incremental_bonus_pot_end = this._incremental_bonus_pot_end + _totalAward;
            this.bonuspot_number = this._incremental_bonus_pot_end;
            this._incremental_bonus_pot = this._incremental_bonus_pot_end;
        }
        else {
            this.bonuspot_number = value.result;
        }
    }
    if (this.$bottomUI) {
        this.$bottomUI.playPayoutEffect();
    }
    this.stopEffectSound("snd_payout_" + this.payoutType);
    if (this.payoutType == "a1" || this.payoutType == "b1") {
        this.stopBGMSound();
    }
    this.playEffectSound("snd_payout_" + this.payoutType + "_end");
    this.changeBGMVolume(1, 1);
    super.onPayoutCompleteValue(value);
    this.$bottomUI.num_payout.visible = false;
}

/**
 * 슬롯 개별 오버라이드 용 잭팟 발생후에 해야할게 있으면 오버라이드 해서 사용한다.
 *
 */
onJackpotEnd()
{
    this.pausedBGMSound(false);
    super.onJackpotEnd();
}

////////////////////////////////////////////////////////////////////////////////////////////
// 복구
////////////////////////////////////////////////////////////////////////////////////////////
_onCheckBonusMode()
{
    super._onCheckBonusMode();
}

/**
 * _onCheckBonusModeSevralProc
 * 슬롯 진입시 보너스 모드 체크하여 보너스 보전
 */
_onCheckBonusModeSevralProc()
{
    this._currentSpinType = this._slotLoginInfo.slotState.nextSpinType;
    let _featuredMap = this.getFeaturedMap();
    if (this.hasNextSpinType(CAnimateSlotBase.SPIN_MODE.FREESPIN)) {
        this.isRecovery = true;
        this.spinType = CSlot346PiggyAdventure.SPIN_TYPE.BONUS;
        this.getFeaturedInfo();
        if (_featuredMap.total_count) {
            let _featuredInfo = _featuredMap.total_count;
            this.freespinTotalCount = _featuredInfo;
        }
        this.freespinRemainCount = this._slotLoginInfo.slotState.freespinRemainCount;
        this.setSlotMode("bonus");
        this.playSlotStayForBonus();
        this.recoveryProcForBonus();
        this.setFreespinLeftcount(this.freespinRemainCount, this.freespinTotalCount);
        this.setOnlyBonusPotPay(false);
        this.$bottomUI.setGoodLuck();
    }
    if (this.hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN)) {
        if (DUG.common.settings.SLOT_HAS_TOPBAR) {
            this._isAvailableClickTopbar = false;
            this.$topBar.checkSpinStatusUI({isRunSpin : true});
        }
        this.getFeaturedInfo();
        this.setBottomUI(false);
        if (_featuredMap.FEATURED_KEY_PICK_TYPE) {
            this.pickType = _featuredMap.FEATURED_KEY_PICK_TYPE;
        }
        this.isRecovery = true;
        this.spinType = this.pickType + 1;
        let type = String.fromCharCode(this.pickType + 96);
        let _type = "normal";

        this._changeTotalBet();
        if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT) {
            this.superFreespinRemainCount = _featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT;
        }
        if (this.superFreespinRemainCount > 0) {
            this.isSuperFreespin = true;
            if (_featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT) {
                this.pickCount = _featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT;
            }
            this.setSuperIn(type);
            if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
                this.recoveryProcForPyramid();
            }
            else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
                this.recoveryProcForAtlantis();
            }
            else if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
                this.recoveryProcForOriental();
            }
        }
        else {
            if (_featuredMap.FEATURED_KEY_KEY_COUNT) {
                this.pickCount = _featuredMap.FEATURED_KEY_KEY_COUNT;
            }
            this.isPick = true;
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
            this.setChangeBG(true, type, _type);
            this.makeInfoPig(true);
        }
        if (this.pickCount != 0) {
            this.makeUiKey();
        }
        this.setOnlyBonusPotPay(false);
        this.$bottomUI.setGoodLuck();
        this.setAvgTotalBet();
    }
    if (this.isRecovery) {
        this.pickStart = false;
        this.superFreespinStart = false;
        if (this.isPick) {
            if (!this.isControlByOther()) {
                if (DUG.common.settings.SLOT_HAS_TOPBAR) {
                    this._isAvailableClickTopbar = false;
                    this.$topBar.checkSpinStatusUI({isRunSpin : true});
                }
                this.$spinProcessQueue.add(this.onWaitPick.bind(this));
                this.$spinProcessQueue.next(500);
            }
        }
        else {
            super._onCheckBonusModeSevralProc();
        }
    }
}

_onInitializedBottomUIAfter()
{
    if (this.hasNextSpinType(CAnimateSlotBase.SPIN_MODE.RESPIN)) {
        this.setAvgTotalBet();
        if (DUG.common.settings.SLOT_HAS_TOPBAR) {
            this._isAvailableClickTopbar = false;
            this.$topBar.checkSpinStatusUI({isRunSpin : true});
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 공통 함수(재구현)
////////////////////////////////////////////////////////////////////////////////////////////
_createSlotReelWrapper(instance, options)
{
    options.onBounceStopSpin = this._onBounceStopSpin.bind(this);
    // 첫 릴스핀 시작위치 랜덤 설정
    let randomStart = CMiscUtils.getRandomIntInclusive(0, options.reelArray.length - 1);
    options.startPosition = randomStart;
    options.spineContainer = this.$spineContainer;
    options.findIndexFunc = this.findIndexFunc;
    instance.slot = this;
    return new CSlot346PiggyAdventureReelWrapper(instance, options);
}

totalAward(superEnd = false)
{
    let _totalAward = this._currentResult.spinResult.payout.totalAward;
    let jackpotAward = this._currentResult.spinResult.payout.jackpotAward || 0;
    let award = _totalAward - jackpotAward;
    // if (superEnd && this.superTotalAward > 0) {
    // 	award = this.superTotalAward;
    // }
    return award;
}

/**
 * @override
 * 스핀 멈추면서 반동할때 호출.
 */
_onBounceStopSpin($slotReelWrapper)
{
    const reelIndex = $slotReelWrapper.reelIndex;
    // 릴 스핀 종료 재생
    let _resultIcons = $slotReelWrapper.getResultIcons();
    let freespinScatterAppear = false;
    let pickScatterAppear = false;
    let isOn = false;
    for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
        let $icon = _resultIcons[rowIndex];
        let _iconId = $icon._dug_icon_id;
        if (!this.isNormalScatter(_iconId)) {
            continue;
        }
        if (_iconId >= 30) {
            if (reelIndex == 2 && this._pickScatterAppearCount == 0) {
                isOn = true;
            }
            else if (reelIndex == 3 && this._pickScatterAppearCount <= 1) {
                isOn = true;
            }
        }
        if (_iconId < 30) {
            if (reelIndex == 2 && this._bonusScatterAppearCount == 0) {
                isOn = true;
            }
            else if (reelIndex == 4 && this._bonusScatterAppearCount <= 1) {
                isOn = true;
            }
        }
        if (isOn) {
            continue;
        }
        if (_iconId >= 30) {
            pickScatterAppear = true;
            this._pickScatterAppearCount++;
        }
        else {
            this._bonusScatterAppearCount++;
            freespinScatterAppear = true;
        }
        let target = this.allReelFrame[this.spinType].animation;
        if (rowIndex != 0 && rowIndex != 8) {
            target = this.$slot.layer_reel_up;
        }
        let _action = "appear";
        let $aIcon = this._playIconAnimation({
            icon : $icon,
            action : _action,
            loop : false,
            animationTarget : target,
            reelIndex : reelIndex,
            rowIndex : rowIndex
        });
    }
    this.playEffectSound("snd_reel_stop");

    if (freespinScatterAppear && !pickScatterAppear) {
        this.playEffectSound("snd_scatter_appear_" + this._bonusScatterAppearCount);
    }
    else if (pickScatterAppear) {
        this.playEffectSound("snd_scatter_appear_6");
    }
}

/**
 * @protected
 * 아이콘 논히트 처리
 */
_playIconNoHitAnimation($icon, _alpha = 0.2)
{
    if ($icon.setAnimation) {
        $icon.setAnimation("on");
    }
    $icon.alpha = _alpha;
}

getFeaturedInfo()
{
    let _featuredMap = this.getFeaturedMap();
    this.fixedPlaylineForBonus = this.create1DArray(this._bonusReelWidth, 0);
    this.fixedPayoutForBonus = this.create1DArray(this._bonusReelWidth, 0);
    this.fixedPlaylineForSuper = this.create2DArray(this._reelWidth, this._reelWindowHeight, 0);
    this.fixedPayoutForSuper = this.create2DArray(this._reelWidth, this._reelWindowHeight, 0);
    this.symbolPayoutForSuper = 0;
    this.boxResultForFreespin = [];
    if (!_featuredMap) {
        return;
    }
    if (_featuredMap.FEATURED_KEY_PICK_TYPE) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_PICK_TYPE;
        this.pickType = _featuredInfo;
    }
    if (_featuredMap.FEATURED_KEY_PICK_RESULT_LIST) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_PICK_RESULT_LIST;
        for (let index = 1; index < 4; index++) {
            this.pickResultInfo[`$ { index }`] = _featuredInfo[index];
        }
    }
    if (_featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_REMAIN_SUPER_BONUS_SPIN_COUNT;
        this.superFreespinRemainCount = _featuredInfo;
        if (this.superFreespinRemainCount > 0 && !this.isSuperFreespin && !this.isRecovery) {
            this.superFreespinStart = true;
        }
    }
    if (_featuredMap.FEATURED_KEY_KEY_COUNT) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_KEY_COUNT;
        if (_featuredInfo > 0 && this.pickCount == 0 && !this.isRecovery) {
            this.pickCount = _featuredInfo;
            this.pickStart = true;
        }
    }
    if (_featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_REMAIN_PICK_COUNT;
        this.pickRemainCount = _featuredInfo;
    }

    if (_featuredMap.FEATURED_KEY_FIX_PLAYLINES) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_FIX_PLAYLINES;
        for (let i = 0; i < _featuredInfo.length; i++) {
            for (let j = 0; j < _featuredInfo[i].length; j++) {
                let index = i * 9 + j + 1;
                this.fixedPlaylineForBonus[index] = _featuredInfo[i][j];
            }
        }
    }
    if (_featuredMap.FEATURED_KEY_FIX_PAYOUTLINES) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_FIX_PAYOUTLINES;
        for (let i = 0; i < _featuredInfo.length; i++) {
            for (let j = 0; j < _featuredInfo[i].length; j++) {
                let index = i * 9 + j + 1;
                this.fixedPayoutForBonus[index] = _featuredInfo[i][j];
            }
        }
    }
    if (_featuredMap.FEATURED_KEY_DIG_HEIGHT_LIST) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_DIG_HEIGHT_LIST;
        for (let i = 0; i < _featuredInfo.length; i++) {
            this.digHeightInfo[i] = _featuredInfo[i];
        }
    }
    if (_featuredMap.FEATURED_KEY_CHEST_LIST) {
        let _featuredInfo = _featuredMap.FEATURED_KEY_CHEST_LIST;
        for (let i = 0; i < _featuredInfo.length; i++) {
            this.boxResultForFreespin[i] = _featuredInfo[i];
        }
    }
    // if (_featuredMap.total_count) {
    // 	let _featuredInfo = _featuredMap.total_count;
    // 	this.freespinTotalCount = _featuredInfo;
    // }
    if (this.isSuperFreespin) {
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.PYRAMID) {
            if (_featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES) {
                let _featuredInfo = _featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES;
                for (let i = 0; i < _featuredInfo.length; i++) {
                    for (let j = 0; j < _featuredInfo[i].length; j++) {
                        this.fixedPlaylineForSuper[i][j] = _featuredInfo[i][j];
                    }
                }
            }
            if (_featuredMap.FEATURED_KEY_FIX_PAYOUTLINES) {
                let _featuredInfo = _featuredMap.FEATURED_KEY_FIX_PAYOUTLINES;
                for (let i = 0; i < _featuredInfo.length; i++) {
                    for (let j = 0; j < _featuredInfo[i].length; j++) {
                        this.fixedPayoutForSuper[i][j] = _featuredInfo[i][j];
                    }
                }
            }
            if (_featuredMap.FEATURED_KEY_SUPER_SYMBOL_PAYOUT) {
                let _featuredInfo = _featuredMap.FEATURED_KEY_SUPER_SYMBOL_PAYOUT;
                this.symbolPayoutForSuper = _featuredInfo;
            }
        }
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ATLANTIS) {
        if (_featuredMap.respin_status) {
            let _featuredInfo = _featuredMap.respin_status;
            this.spinStatusForAtlantis = _featuredInfo;
        }
        if (_featuredMap.respin_award) {
            let _featuredInfo = _featuredMap.respin_award;
            this.respinAwardForAtlantis = _featuredInfo;
        }
        if (_featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES) {
            let _featuredInfo = _featuredMap.FEATURED_KEY_SUPER_RESPIN_PLAYLINES;
            for (let i = 0; i < _featuredInfo.length; i++) {
                for (let j = 0; j < _featuredInfo[i].length; j++) {
                    this.fixedPlaylineForSuper[i][j] = _featuredInfo[i][j];
                }
            }
        }
    }
    if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.ORIENTAL) {
        if (_featuredMap.feature) {
            let _featuredInfo = _featuredMap.feature;
            for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
                for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
                    if (_featuredInfo[reelIndex][rowIndex]) {
                        this.fixedPayoutForSuper[reelIndex][rowIndex] = _featuredInfo[reelIndex][rowIndex];
                    }
                }
            }
        }
        if (_featuredMap.respin_payout) {
            this.superTotalAward = _featuredMap.respin_payout;
        }
    }
}

/**
 * 슬롯 모드를 설정한다.
 */
setSlotMode(type)
{
    if (this.$bottomUI) {
        this.$bottomUI.setMode(type);
    }
    this.checkButtonStatus();
    this.setReelVisible(this.spinType);
    if (type == 'bonus') {
        this.setReelArrayToReel(1);
        if (this.spinType == CSlot346PiggyAdventure.SPIN_TYPE.BONUS) {
            this.$slot.bg.gotoAndStop('bonus');
            this.$slot.reel_bg.gotoAndStop('bonus');
        }
        this.bonuspot_number = 0;
        this.removeUiScroll();
    }
    else {
        this.setReelArrayToReel(0);
        this.payout_number = 0;
        this.$bottomUI.area_payout.visible = false;
        this.$slot.bg.gotoAndStop('normal');
        this.$slot.reel_bg.gotoAndStop('normal');
    }
}

set jackpot_number(value)
{
    super.jackpot_number = value;
    this.setMultiPotText();
}

_changeTotalBet()
{
    if (this._customFreespinInfo) {
        return;
    }
    super._changeTotalBet();
    let _featuredMap = this.getFeaturedMap();
    if (_featuredMap && _featuredMap.aveTB) {
        let _featuredInfo = _featuredMap.aveTB;
        this._avgTotalBet = Math.floor(_featuredInfo);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 공통 함수
////////////////////////////////////////////////////////////////////////////////////////////
setAvgTotalBet()
{
    this.$bottomUI.num_totalbet.text = 'AVG';
    this.$bottomUI.num_totalbet.setAutoAlign();
}

setBottomUI(bool)
{
    this.$bottomUI.btn_spin.interactive = bool;
    this.$bottomUI.btn_maxbet.interactive = bool;
    this.$bottomUI.btn_betplus.interactive = bool;
    this.$bottomUI.btn_betminus.interactive = bool;
    if (bool) {
        this.$bottomUI.setEnableBetBtn();
        this.$bottomUI.setEnableMaxBet();
        this.$bottomUI.setEnableSpinBtn();
    }
    else {
        this.$bottomUI.setDisableBetBtn();
        this.$bottomUI.setDisableMaxBet();
        this.$bottomUI.setDisableSpinBtn();
    }
}

setBonustime(visible)
{
    this.$bottomUI.setBonustimeBtn(visible);
    if (this.isFreespin) {
        this.$bottomUI.area_skip.visible = true;
    }
    this.$bottomUI.btn_spin.visible = !visible;
    this.$bottomUI.btn_spinleft.visible = !visible;
    this.setBottomUI(!visible);
}

/**
 * 프리스핀 잔여횟수를 설정한다.
 *  @protected
 * @param {number} leftcount - free spin left count
 */
setFreespinLeftcount(leftcount, totalCount)
{
    if (this.$bottomUI) {
        this.$bottomUI.setSpinsLeft(leftcount, false, totalCount);
    }
    else if (this.$slot.bg.number_spinsleft && this.$slot.bg.text_spinleft) {
        this.$slot.bg.number_spinsleft.$text.text = leftcount;
        this.$slot.bg.text_spinleft.gotoAndStop(leftcount != 1 ? 'spins' : 'spin');
    }
}
/**
 * 스핀 시 차감 정보 작성
 * @protected
 */
_setSpinDeductions()
{
    // 금액 차감
    if (this.isNextSpinNoMoneyIn() == = false) {
        this.mychips_number = this.mychips_number - this._totalBet;
    }
}

playEffectSpine(spineName, _parent, action)
{
    let spine = this.$spineContainer.borrow(spineName);
    _parent.addChild(spine);
    spine.setAnimation(action);
    spine.addAnimationComplete(
        action, (entry) = > {
            co(function * () {
                try {
                    yield wait(0);
                    if (this._is_destroy) {
                        return;
                    }
                    if (spine && spine.parent) {
                        spine.parent.removeChild(spine);
                        spine = null;
                    }
                }
                catch (error) {
                    DUG.log(error);
                }
            }.bind(this));
        });
}

// z값으로 정렬
depthCompare(a, b)
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

getSlotLocalPosision($spine, slotName)
{
    let bone = $spine.getBone(slotName);
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
playPayoutIncrement(options)
{
    this.$timeline_payout.timeScale(1);
    let _value = {value : 0, init : options.init, result : options.result, options : options};
    this.$timeline_payout.to(_value, options.duration, {
        value : 1,
        onUpdate : this.onPayoutUpdateValueBindMe ? this.onPayoutUpdateValueBindMe : this.onPayoutUpdateValueBindMe = this.onPayoutUpdateValue.bind(this),
        onUpdateParams : [_value],
        onComplete : this.onPayoutCompleteValueBindMe ? this.onPayoutCompleteValueBindMe : this.onPayoutCompleteValueBindMe = this.onPayoutCompleteValue.bind(this),
        onCompleteParams : [ _value, options.onComplete ],
    });
}

setOnlyBonusPotPay(visible)
{
    if (visible) {
        this._incremental_bonus_pot = 0;
        this._incremental_bonus_pot_end = 0;
    }
    this.bonuspot_number = 0;
    this._bUse_update_bonus_pot = visible;
    this.$bottomUI.num_bonuspot.visible = visible;
    this.$bottomUI.num_payout.visible = false;
}

_initAllAnimations()
{
    this._setResultIconsAlpha(1);
    this.hideAllPaylines();
    this._releaseAllReelAnimation();
    this._releaseAnimationIcons();
    if (this.removeHitBoxFlag) {
        this.removeHitBoxForNormal();
        this.removeHitBoxFlag = false;
    }
}

create1DArray(columns, value)
{
    let arr = [];
    for (let i = 0; i < columns; i++) {
        arr[i] = value;
    }
    return arr;
}

create2DArray(columns, rows, value)
{
    let arr = [];
    for (let i = 0; i < columns; i++) {
        arr[i] = [];
        for (let j = 0; j < rows; j++) {
            arr[i][j] = value;
        }
    }
    return arr;
}

create3DArray(columns, rows, num, value)
{
    let arr = [];
    for (let i = 0; i < columns; i++) {
        arr[i] = [];
        for (let j = 0; j < rows; j++) {
            arr[i][j] = [];
            for (let x = 0; x < num; x++) {
                arr[i][j][x] = value;
            }
        }
    }
    return arr;
}

hideNeedlessIcons()
{
    for (let reelIndex = 0; reelIndex < this._reelWidth; reelIndex++) {
        let $slotReelWrapper = this.slotReels[reelIndex];
        for (let rowIndex = 0; rowIndex < this._reelWindowHeight; rowIndex++) {
            $slotReelWrapper.hideNeedlessIcon();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// 페이테이블
////////////////////////////////////////////////////////////////////////////////////////////
_createPaytable()
{
    this.$paytable = new CAnimatePaytable({
        type : this._slotConfig.slot_type,
        payouts : this._slotInfo.payoutArray,
        setSymbolPayout : this.setSymbolPayoutCustomBindMe ? this.setSymbolPayoutCustomBindMe : this.setSymbolPayoutCustomBindMe = this.setSymbolPayoutCustom.bind(this)
    });

    super._createPaytable(this.$paytable);
}

setSymbolPayoutCustom(options)
{
    let _number = Math.floor(options.unitBet * options.payouts[options.symbolID][options.hitcount]);

    options.text.text = options.hitcount + ' - ' + CMiscUtils.abbreviatesNumberOption({num : _number});
}

/**
 * 페이테이블 블록레이어 생성
 */
_createPaytableBlocklayer()
{
    this.$paytable_block_layer = new PIXI.Graphics();
    this.$paytable_block_layer.beginFill(0x000000);
    this.$paytable_block_layer.drawRect(0, 0, DUG.common.settings.RENDERER_WIDTH, DUG.common.settings.RENDERER_HEIGHT);
    this.$paytable_block_layer.endFill();
    this.$paytable_block_layer.interactive = true;
    this.$paytable_block_layer.alpha = 0.7;

    this.addChild(this.$paytable_block_layer);
}

////////////////////////////////////////////////////////////////////////////////////////////
// 페이 라인
////////////////////////////////////////////////////////////////////////////////////////////
/**
 * 페이라인 정의
 * @override
 */
_getPayLines()
{
    let paylines = [
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
_setWinLines()
{
    let borderColors = this.options.paylines.borderColors || [0x3333DD];
    let borderWidth = this.options.paylines.borderWidth || 6;
    let borderAlpha = this.options.paylines.borderAlpha || 1;
    let innerColors = this.options.paylines.innerColors || [0xFFFFFF];
    let innerWidth = this.options.paylines.innerWidth || 0;
    let innerAlpha = this.options.paylines.innerAlpha || 1;

    let splitLines = this.options.paylines.splitLines || false;

    let plusLength = this.options.paylines.sidePlusLength || 0;

    let _paylineArr = this._getPayLines();
    if (!_paylineArr) {
        DUG.Log('_getPayLines 함수가 정의되지 않음');
        return;
    }
    let usingCheck = [];
    let unitLineCount = parseInt(this._lineCount / this._reelWindowHeight) * 2 + 1;

    if (splitLines == = true) {
        this._readyWinlineSplit(usingCheck, unitLineCount);
    }
    for (let i = 0, length = this._lineCount; i < length; ++i) {
        let positions = [];
        let fixedRow = [];
        let usingIndex = 0;
        for (let reelIndex = -1; reelIndex < this._reelWidth + 1; reelIndex++) {
            let addValue = 0;
            let x = reelIndex;
            if (reelIndex < 0) {
                x = 0;
                addValue = (this._iconWidth + plusLength) * -0.5;
            }
            else if (reelIndex == this._reelWidth) {
                x = this._reelWidth - 1;
                addValue = (this._iconWidth + plusLength) * 0.5;
            }
            let y = _paylineArr[i][x];
            if (splitLines == = true) {
                if (this._checkStraightLine(reelIndex, _paylineArr, i, x, y) == = true) {
                    continue;
                }
            }
            let slotReelWrapper = this.slotReels[x];
            let _resultIcons = slotReelWrapper.getResultIcons();
            let $icon = _resultIcons[y];

            let resultY = this.$slot.layer_reel_frame.y + (this._iconHeight * y) + (this._iconHeight * 0.5);
            if (splitLines == = true) {
                resultY = this._checkOverlapLine(unitLineCount, fixedRow, usingCheck, y, resultY, usingIndex, i);
            }
            usingIndex++;
            let $reel = this.$slot.layer_reel_frame[`reel_$ { x + 1 }`];
            positions.push({x : this.$slot.layer_reel_frame.x + $reel.x + addValue, y : resultY});
        }
        if (this.$slot.payline[`betline$ { i + 1 }`]) {
            this.$slot.payline[`betline$ { i + 1 }`].parent.removeChild(this.$slot.payline[`betline$ { i + 1 }`]);
            this.$slot.payline[`betline$ { i + 1 }`].destroy({children : true});
            delete this.$slot.payline[`betline$ { i + 1 }`];
        }
        if (!this.$slot.payline[`betline$ { i + 1 }`]) {
            this.$slot.payline[`betline$ { i + 1 }`] = new PIXI.Container();
            this.$slot.payline.addChild(this.$slot.payline[`betline$ { i + 1 }`]);
        }
        this.$slot.payline[`betline$ { i + 1 }`].x = 0;
        this.$slot.payline[`betline$ { i + 1 }`].y = 0;
        if (borderWidth > 0.0) {
            let line = new PIXI.Graphics();
            let color = borderColors.shift();
            line.lineStyle(borderWidth, color, borderAlpha);
            this._drawWinLine(line, positions);
            borderColors.push(color);
            this.$slot.payline[`betline$ { i + 1 }`].addChild(line);
        }
        if (innerWidth > 0.0) {
            let line2 = new PIXI.Graphics();
            let color = innerColors.shift();
            line2.lineStyle(innerWidth, color, innerAlpha);
            this._drawWinLine(line2, positions);
            innerColors.push(color);
            this.$slot.payline[`betline$ { i + 1 }`].addChild(line2);
        }
        //기존 윈라인 처리시 호출되는 함수
        this.$slot.payline[`betline$ { i + 1 }`].gotoAndStop = () = > {};
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// Cheat ###
////////////////////////////////////////////////////////////////////////////////////////////

_makeExtensionData()
{
    let extensionData = super._makeExtensionData();
    if (this.userPick > 0) {
        extensionData.userPick = this.userPick;
    }

    extensionData.testMode = 0;
    if (DUG.common.settings.testMode > 0) {
        extensionData.testMode = DUG.common.settings.testMode;
    }
    extensionData.testMode2 = 0;
    if (DUG.common.settings.testMode2 > 0) {
        extensionData.testMode2 = DUG.common.settings.testMode2;
    }
    extensionData.testMode3 = 0;
    if (DUG.common.settings.testMode3 > 0) {
        extensionData.testMode3 = DUG.common.settings.testMode3;
    }
    extensionData.resetSave = 0;
    if (DUG.common.settings.resetSave > 0) {
        extensionData.resetSave = DUG.common.settings.resetSave;
    }
    return extensionData;
}

onKeyUp(key)
{
    if (this.settingsData && this.settingsData[key.keyCode]) {
        eval(this.settingsData[key.keyCode]);
        return;
    }
    if (key.keyCode == 76) {        // l í¤ ìë ¥
        let settingsURL = `https :  // script.google.com/macros/s/AKfycbxC4DsBHAbcxU-MkAra_myZ0Ecvo1bTd4NMTcfeKy4JD8pqoG8c7CtYbRDde4FYQQZs1Q/exec?sheetName=${this.slotType}`;
                                    AJAX.getJSON(
                                        settingsURL, (data) = > {
                                            if (data.ok) {
                                                if (!this.pixiBasicText) {
                                                    this.pixiBasicText = new PIXI.Text("");
                                                    this.$slot.addChild(this.pixiBasicText);
                                                    this.pixiBasicText.style.fill = 0xFFF000;
                                                    this.pixiBasicText.style.stroke = 0x000000;
                                                    this.pixiBasicText.style.strokeThickness = 5;
                                                    this.pixiBasicText.x = 150;
                                                    this.pixiBasicText.y = 200;
                                                }
                                                this.pixiBasicText.text = "## Cheats Loaded ##\n";

                                                let _settingsData = data.data[this.slotType];
                                                this.settingsData = {};
                                                for (let i = 0, length = _settingsData.length; i < length; ++i) {
                                                    let ascii = _settingsData[i].key.toString().charCodeAt(0);
                                                    this.settingsData[ascii] = _settingsData[i].value;

                                                    let keyStr = _settingsData[i].key.toString();
                                                    let desc = _settingsData[i].desc;

                                                    this.pixiBasicText.text += `${keyStr} : $ { desc }
                                                    \n`;
                                                }
                                                DUG.log('load success settings infomation');

                                                co(function * () {
                                                    try {
                                                        yield wait(5000);
                                                        if (this._is_destroy) {
                                                            return;
                                                        }
                                                        this.pixiBasicText.text = "";
                                                    }
                                                    catch (error) {
                                                        DUG.log(error);
                                                    }
                                                }.bind(this));
                                            }
                                        });
    }
    super.onKeyUp(key);
}

CSlot346PiggyAdventure.SPIN_TYPE = {
    NORMAL : 0,
    BONUS : 1,
    PYRAMID : 2,
    ATLANTIS : 3,
    ORIENTAL : 4,
    PICK : 5
};
`betline$ { i + 1 }
`
`$ { a + 1 }
`;

`num_$ { index + 1 * 3 / 2 % 1 }
`;
`${index + 1 * 3 / 2 % 1} _num`;
`num_${index + 1 * 3 / 2 % 1} oZszz
`;
_openBoxFrame[reelIndex].parent.removeChild(_openBoxFrame[reelIndex]);

CAnimateSlotBase::playIcon_options options(_icon, _action, _loop);
options.aniName = target;
options.animationTarget = target;
auto _aIcon = _playIconAnimation(options);



auto iconInfo = SlotAddInfo::get(_aIcon);
auto boxInfo = SlotAddInfo::get(_aIcon);
iconInfo->setInfo("reelIndex", reelIndex);
iconInfo->setInfo("rowIndex", rowIndex);

layer_reel_frame->retain();
layer_reel_frame->removeFromParentAndCleanup(false);
customReelMask->addChild(layer_reel_frame);
layer_reel_frame->release();
layer_reel_frame->setPosition(reelPosX, reelPosY);
StringUtils::format("", );
auto addIcon = _icon->getVariable<CSimpleCCBI*>("addIcon");
if (addIcon) {
    addIcon->setAnimation(action);
}

for (auto i = 0, size = array.size(); i < size; i++) {
    openState = false;
}

for (auto i = 0, size = array.size(); i < size; i++) {
    openState = 0;
}

CSlotReelWrapper* CSlot338HooHooHooMonkey::_createSlotReelWrapper(CReelFrame* instance, SlotReelWrapper::Options options)
{
    int randomStart = CMiscUtils::getRandomIntInclusive(0, options.reelArray.size() - 1);

    options.startPosition = randomStart;
    options.reelArray = _setReelRandomSymbol(0, options.reelArray);
    options.findIndexFunc = CC_CALLBACK_0(CSlot338HooHooHooMonkey::findIndexFunc, this);
    options.onBounceStopSpin = CC_CALLBACK_1(CSlot338HooHooHooMonkey::onBounceStopSpin, this);
    CSlot338HooHooHooMonkeyReelWrapper::SlotInfo info;
    info.slot = this;
    auto reelWrapper = CSlot338HooHooHooMonkeyReelWrapper::create(instance, options, info);
    instance->addChild(reelWrapper);
    return reelWrapper;
}

CAnimateSlotBase::onPayout_options options;
options.duration = _duration;
options.totalAward = _totalAward;
options.init = _init;
options.nextQueueDelay = 2.0f;

_fixedSymbol = vector<vector<CSimpleCCBI*>>(7, vector<CSimpleCCBI*>(6, nullptr));
_tresureBox = vector<CSimpleCCBI*>(30, nullptr);
