
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_PAGE_UP, QKeySequence(Qt::Key_PageUp));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_PAGE_DOWN, QKeySequence(Qt::Key_PageDown));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_BUY1, QKeySequence(Qt::Key_F1));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_BUY2, QKeySequence(Qt::Key_F2));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_BUY3, QKeySequence(Qt::Key_F3));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_SELL1, QKeySequence(Qt::Key_F4));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_SELL2, QKeySequence(Qt::Key_F5));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_SELL3, QKeySequence(Qt::Key_F6));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_LAST_ORDER, QKeySequence("Alt+C"));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_ORDER, QKeySequence("Alt+V"));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_STOCK, SCD_QUICKORDER_OPT_TRY_ORDER, QKeySequence(Qt::Key_Enter));

            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_PAGE_UP, QKeySequence(Qt::Key_PageUp));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_PAGE_DOWN, QKeySequence(Qt::Key_PageDown));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_BUY1, QKeySequence(Qt::Key_F1));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_BUY2, QKeySequence(Qt::Key_F2));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_BUY3, QKeySequence(Qt::Key_F3));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_SELL1, QKeySequence(Qt::Key_F4));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_SELL2, QKeySequence(Qt::Key_F5));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_SELL3, QKeySequence(Qt::Key_F6));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_LAST_ORDER, QKeySequence("Alt+C"));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_ORDER, QKeySequence("Alt+V"));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_TRY_ORDER, QKeySequence(Qt::Key_Enter));
            setDefaultKeySetting(smtKeySetting, ttservice::TDT_CREDIT, SCD_QUICKORDER_OPT_TRY_SLO_SELL, QKeySequence(Qt::Key_F7));
            
    void CShortcutManager::keyPressEvent(int nPanelGroup, int nTraderType, QKeyEvent* e)
    {
        if (!LMC(string("mdl_order") << "mdl_order_shortcut"))
        {
            return;
        }
        //if (!getUserConfig()->m_smartShortCutSetting.m_bBasicShortcut)
        //{
        //    return;
        //}
        int uKey = e->key();

        bool bModifier = false;
        QString qsOpAvailable = "";
        Qt::KeyboardModifiers modifiers = e->modifiers();
        if (modifiers & Qt::ShiftModifier)
        {
            uKey += Qt::SHIFT;
            bModifier = true;
        }
        if (modifiers & Qt::ControlModifier)
        {
            uKey += Qt::CTRL;
            bModifier = true;
        }
        if (modifiers & Qt::AltModifier)
        {
            if (48 <= uKey && uKey <= 57
                && (ID_PANEL_QUICK_ORDER != nPanelGroup && getUserConfig()->m_smartShortCutSetting.getOpAvailable((ttservice::EXtTraderType)nTraderType)
                    || (ID_PANEL_QUICK_ORDER == nPanelGroup && getUserConfig()->m_smartShortCutSetting.getQuickOrderOpAvailable()))
            )
            {
                qsOpAvailable = uKey;
            }
            uKey += Qt::ALT;
            bModifier = true;
        }

        if (!qsOpAvailable.isEmpty())
        {
            doActivateWidget(nPanelGroup, nTraderType, qsOpAvailable);
        }
        else if (Qt::Key_Up == uKey || Qt::Key_Down == uKey || Qt::Key_Left == uKey || Qt::Key_Right == uKey)
        {
            doActivateWidgetByDirect(nPanelGroup, nTraderType, uKey);
        }
        else if (getUserConfig()->m_smartShortCutSetting.m_bBasicShortcut && XP::isKeyCharOrNum(e) && !bModifier)
        {
            // 处理智能输入股票
            if (getUserConfig()->m_smartShortCutSetting.m_bSmartInputCode)
            {
                XP::IDockableWidget* targetDock = NULL;

                QVector<IDockWindow*> vecWidget = XP::CDockWindowManager::instance()->getWindow(nPanelGroup);
                for (IDockWindow* pDockWindow : vecWidget)
                {
                    if (NULL != pDockWindow && NULL != pDockWindow->dockPanel())
                    {
                        QVector<XP::IDockableWidget *> dockables = pDockWindow->dockPanel()->getDockableWidgets();
                        for (int i = 0; i < dockables.size(); i++)
                        {
                            if (NULL != dockables[i] && dockables[i]->id() == DID_OPERATION)
                            {
                                ITradeDockableWidget *pDockWidget = dynamic_cast<ITradeDockableWidget *>(dockables[i]);
                                if (NULL != pDockWidget && pDockWidget->tradeType() == nTraderType)
                                {
                                    if (pDockWidget->isWindowFocusWidget())
                                    {
                                        if (pDockWidget->isActive())
                                        {
                                            pDockWidget->handleShortcut(-1, e);
                                        }
                                        targetDock = NULL;
                                        return;
                                    }
                                    else if (NULL == targetDock)
                                    {
                                        targetDock = dockables[i];
                                    }
                                }
                            }
                        }
                    }
                }
                if (NULL != targetDock && targetDock->isActive())
                {
                    targetDock->handleShortcut(-1, e);
                }
            }
        }
        else
        {
            //int nShortType = m_shortCutSetting.m_mapBasicKeySequence.key(QKeySequence(uKey), -1);
            //if (nShortType != -1 && getUserConfig()->m_smartShortCutSetting.m_bGlobleShortcut && LMC(string("mdl_order") << "mdl_order_shortcut") && LMC(string("mdl_order") << "mdl_order_keyboard_hook"))
            //{
            //    return;//基本快捷键且勾选全局热键时，由全局钩子响应时间，此处return
            //}

            if (ID_PANEL_QUICK_ORDER != nPanelGroup)
            {
                //处理针对交易界面设置的全局快捷键
                int nShortCutType = m_shortCutSetting.m_mapBasicKeySequence.key(QKeySequence(uKey), -1);
                if (nShortCutType != -1)
                {
                    QMap<int, bool>& mapDatas = getUserConfig()->m_controlVisibleSettingArr[(ttservice::EXtTraderType)nTraderType];
                    if (
                        !((nShortCutType == SCD_TRADEORDER_CANCEL_BUY && !mapDatas.value(ID_ROW_TODAYORDER_CAST_BUY))
                        || (nShortCutType == SCD_TRADEORDER_CANCEL_SELL && !mapDatas.value(ID_ROW_TODAYORDER_CAST_SELL))
                        || (nShortCutType == SCD_TRADEORDER_CANCEL_ALL && !mapDatas.value(ID_ROW_TODAYORDER_CAST_ALL))
                        || (nShortCutType == SCD_TRADEORDER_CANCEL_LAST && !mapDatas.value(ID_ROW_TODAYORDER_CANCEL_LAST))
                        || (nShortCutType == SCD_TRADEORDER_CLEAR_POSITION && !mapDatas.value(ID_ROW_TODAYHOLD_EMPTYSELL_ALL) && getUserConfig()->hasClearPositionAuth()))
                        )
                    {
                        emit sigHandleTradeGlobalShortCut(nShortCutType);
                        return;
                    }
                }
            }
            // 处理设置的快捷键
            handleShortcutKey(nPanelGroup, nTraderType, QKeySequence(uKey), e);
            XT_OBJLOG(LLV_INFO, BF("make response to the shortCut %1%") % uKey);
        }
    }
