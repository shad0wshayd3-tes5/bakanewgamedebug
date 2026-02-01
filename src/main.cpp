namespace Hooks
{
	class hkGetFormByNumericID
	{
	public:
		static RE::TESForm* GetFormByNumericID(RE::FormID a_formID)
		{
			auto form = _GetFormByNumericID(a_formID);

			REX::INFO("--- GetFormByNumericID ---");
			REX::INFO("Input FormID:					{:08X}", a_formID);
			if (form)
			{
				REX::INFO("Valid Form?:						{}", true);
				REX::INFO("Source Plugin:					{}", form->GetFile()->GetFilename());
				REX::INFO("FormID:							{:08X}", form->formID);
				REX::INFO("FormType:						{}", form->GetFormType());

				if (auto quest = form->As<RE::TESQuest>())
				{
					REX::INFO("--- Flags ---");
					REX::INFO("raw:								{:016X}", quest->data.flags.underlying());
					REX::INFO("kEnabled							{}", quest->data.flags.all(RE::QuestFlag::kEnabled));
					REX::INFO("kCompleted						{}", quest->data.flags.all(RE::QuestFlag::kCompleted));
					REX::INFO("kAddIdleToHello					{}", quest->data.flags.all(RE::QuestFlag::kAddIdleToHello));
					REX::INFO("kAllowRepeatStages				{}", quest->data.flags.all(RE::QuestFlag::kAllowRepeatStages));
					REX::INFO("kStartsEnabled					{}", quest->data.flags.all(RE::QuestFlag::kStartsEnabled));
					REX::INFO("kDisplayedInHUD					{}", quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD));
					REX::INFO("kFailed							{}", quest->data.flags.all(RE::QuestFlag::kFailed));
					REX::INFO("kStageWait						{}", quest->data.flags.all(RE::QuestFlag::kStageWait));
					REX::INFO("kRunOnce							{}", quest->data.flags.all(RE::QuestFlag::kRunOnce));
					REX::INFO("kExcludeFromExport				{}", quest->data.flags.all(RE::QuestFlag::kExcludeFromExport));
					REX::INFO("kWarnOnAliasFillFailure			{}", quest->data.flags.all(RE::QuestFlag::kWarnOnAliasFillFailure));
					REX::INFO("kActive							{}", quest->data.flags.all(RE::QuestFlag::kActive));
					REX::INFO("kRepeatsConditions				{}", quest->data.flags.all(RE::QuestFlag::kRepeatsConditions));
					REX::INFO("kKeepInstance					{}", quest->data.flags.all(RE::QuestFlag::kKeepInstance));
					REX::INFO("kWantDormant						{}", quest->data.flags.all(RE::QuestFlag::kWantDormant));
					REX::INFO("kHasDialogueData					{}", quest->data.flags.all(RE::QuestFlag::kHasDialogueData));
				}
			}
			else
			{
				REX::INFO("Valid Form?:						{}", false);
			}

			return form;
		}

		inline static REL::Hook _GetFormByNumericID{ REL::ID(52118), 0x39, GetFormByNumericID };
	};

	class hkEnsureQuestStarted
	{
	public:
		static bool EnsureQuestStarted(RE::TESQuest* a_this, bool& a_outStartDelayed, bool a_immediate)
		{
			auto result = _EnsureQuestStarted(a_this, a_outStartDelayed, a_immediate);

			REX::INFO("--- EnsureQuestStarted ---");
			REX::INFO("Result:							{}", result);
			REX::INFO("StartDelayed:					{}", a_outStartDelayed);

			return result;
		}

		inline static REL::Hook _EnsureQuestStarted{ REL::ID(52118), 0x59, EnsureQuestStarted };
	};

	class hkCheckModsLoaded
	{
	public:
		static bool CheckModsLoaded(void* a_this, bool a_everModded)
		{
			auto result = _CheckModsLoaded(a_this, a_everModded);

			REX::INFO("--- CheckModsLoaded ---");
			REX::INFO("Result:							{}", result);
			// REX::INFO("StartDelayed:					{}", a_outStartDelayed);

			return result;
		}

		inline static REL::Hook _CheckModsLoaded{ REL::ID(52118), 0x7C, CheckModsLoaded };
	};

	class hkFillAliases
	{
	public:
		static bool FillAliases(RE::TESQuest* a_this, const void* a_triggerEvent, void* a_timer, bool& a_outTimeOut)
		{
			auto result = _FillAliases(a_this, a_triggerEvent, a_timer, a_outTimeOut);

			REX::INFO("--- FillAliases ---");
			REX::INFO("Quest:							{:08X}", a_this->formID);
			REX::INFO("Result:							{}", result);

			return result;
		}

		inline static REL::Hook _FillAliases{ REL::ID(25003), 0x4F, FillAliases };
	};

	class hkBeginStartUpQuest
	{
	public:
		static void BeginStartUpQuest(void* a_this, RE::TESQuest* a_quest)
		{
			_BeginStartUpQuest(a_this, a_quest);

			REX::INFO("--- BeginStartUpQuest ---");
			REX::INFO("Quest:							{:08X}", a_quest->formID);

			// return result;
		}

		inline static REL::Hook _BeginStartUpQuest{ REL::ID(25003), 0x62, BeginStartUpQuest };
	};

	class hkFindRefsForAlias
	{
	public:
		static RE::ObjectRefHandle* FindRefsForAlias(RE::TESQuest* a_this, RE::ObjectRefHandle* a_result, RE::BGSRefAlias* a_alias, void* a_event, void* a_timer, bool& a_timedOut)
		{
			auto result = _FindRefsForAlias(a_this, a_result, a_alias, a_event, a_timer, a_timedOut);

			if (a_this && a_this->formID == 0x0F02466E)
			{
				REX::INFO("--- FindRefsForAlias ---");
				REX::INFO("idx:								{}", a_alias->aliasID);
				REX::INFO("Alias:							{}", a_alias->aliasName.c_str());
				REX::INFO("filled?:							{}", result->operator bool());
			}

			return result;
		}

		inline static REL::Hook _FindRefsForAlias{ REL::ID(25048), 0x1A8, FindRefsForAlias };
	};

	class hkClearPartialAliasFill
	{
	public:
		static void ClearPartialAliasFill(RE::TESQuest* a_this)
		{
			REX::INFO("--- ClearPartialAliasFill : INIT ---");
			REX::INFO("Quest:							{:08X}", a_this->formID);
			REX::INFO("aliases.size():					{}", a_this->aliases.size());
			REX::INFO("aliasRefMap.size():				{}", a_this->refAliasMap.size());

			{
				REX::INFO("--- ClearPartialAliasFill : ITER ---");

				a_this->aliasAccessLock.LockForRead();

				auto GetAliasName = [&](std::uint32_t a_index) {
					for (auto iter : a_this->aliases)
					{
						if (iter->aliasID == a_index)
						{
							return iter->aliasName.c_str();
						}
					}

					return "failed to lookup name";
				};

				for (auto& iter : a_this->refAliasMap)
				{
					REX::INFO("idx:								{}", iter.first);
					REX::INFO("alias:							{}", GetAliasName(iter.first));
					if (auto ptr = iter.second.get())
					{
						REX::INFO("formID:							{:08X}", ptr->formID);
					}
					else
					{
						REX::INFO("invalid form!");
					}
					REX::INFO("---");
				}

				a_this->aliasAccessLock.UnlockForRead();
			}

			_ClearPartialAliasFill(a_this);

			REX::INFO("--- ClearPartialAliasFill : POST ---");
			REX::INFO("aliases.size():					{}", a_this->aliases.size());
			REX::INFO("aliasRefMap.size():				{}", a_this->refAliasMap.size());

			// return result;
		}

		inline static REL::Hook _ClearPartialAliasFill{ REL::ID(25048), 0x761, ClearPartialAliasFill };
	};
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse, {.trampoline = true });
	return true;
}
