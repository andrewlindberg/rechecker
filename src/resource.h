#pragma once

#define FILE_INI_RESOURCES	"resources.ini"
#define MAX_CMD_LENGTH		128
#define MAX_RANGE_CONSISTENCY	1024

#define RESOURCE_INDEX_BITS	12
#define RESOURCE_MAX_COUNT	(1 << RESOURCE_INDEX_BITS)

enum flag_type_log
{
	LOG_NONE = 0,
	LOG_NORMAL,
	LOG_DETAILED
};

enum arg_type_e
{
	ARG_TYPE_FILE_NAME = 0,
	ARG_TYPE_FILE_HASH,
	ARG_TYPE_CMD_EXEC,
	ARG_TYPE_FLAG,

	MAX_PARSE_ARGUMENT,
};

// buffer for checker list
class CResourceBuffer: public IResourceBuffer
{
public:
	CResourceBuffer(char *filename, char *cmdExec, flag_type_resources flag, uint32 hash, int line, bool bBreak);

	uint32 GetFileHash() const { return m_FileHash; };
	flag_type_resources GetFileFlag() const { return m_Flag; };

	const char *GetFileName() const { return m_FileName; };
	const char *GetCmdExec() const { return m_CmdExec; };
	int GetLine() const { return m_Line; };

	bool IsBreak() const { return m_Break; };
	bool IsDuplicate() const { return m_Duplicate; };
	bool IsAddEx() const { return m_AddEx; };

	void SetDuplicate() { m_Duplicate = true; };
	void SetAddEx() { m_AddEx = true; };

private:
	uint32 m_FileHash;

	flag_type_resources m_Flag;
	int m_Line;

	const char *m_FileName;
	const char *m_CmdExec;

	bool m_Duplicate;	// for to check for duplicate
	bool m_Break;		// do not check a next files
	bool m_AddEx;		// external add file
};

class CResourceFile: public IResourceFile
{
public:
	CResourceFile();
	~CResourceFile();

	void Init();
	void Clear(IGameClient *pClient = NULL);
	void LoadResources();
	int CreateResourceList();
	void Log(flag_type_log type, const char *fmt, ...);
	void PrintLog(IGameClient *pSenderClient, CResourceBuffer *res, flag_type_resources typeFind, uint32 hash);
	bool FileConsistencyResponse(IGameClient *pSenderClient, resource_t *resource, uint32 hash);

	static const char *DuplicateString(const char *str);
	static void ClearStringsCache();

private:
	// buffer for response list
	class CResponseBuffer
	{
	public:
		CResponseBuffer(IGameClient *pSenderClient, char *filename, uint32 hash);

		int GetUserID() const { return m_UserID; };
		IGameClient *GetGameClient() const { return m_pClient; };
		const char *GetFileName() const { return m_FileName; };
		uint32 GetClientHash() const { return m_ClientHash; };

	private:
		int m_UserID;
		IGameClient *m_pClient;
		const char *m_FileName;
		uint32 m_ClientHash;
	};

private:
	// for temporary files of responses
	void AddFileResponse(IGameClient *pSenderClient, char *filename, uint32 hash);
	void LogPrepare();

	// compute the total number of consistency files.
	void ComputeConsistencyFiles();

	// parse
	const char *GetNextToken(char **pbuf);

private:
	typedef std::vector<CResourceBuffer *> ResourceList;
	typedef std::vector<CResponseBuffer *> ResponseList;

	ResourceList m_resourceList;
	ResponseList m_responseList;

	int m_ConsistencyNum;
	uint32 m_PrevHash;

	char m_PathDir[MAX_PATH_LENGTH];
	char m_LogFilePath[MAX_PATH_LENGTH];	// log data

	typedef std::vector<const char *> StringList;
	static StringList m_StringsCache;

public:
	const char *FindFilenameOfHash(uint32 hash);
	int GetConsistencyNum() const { return m_ConsistencyNum; }
	uint32 GetPrevHash() const { return m_PrevHash; }
	CResourceBuffer *AddElement(char *filename, char *cmdExec, flag_type_resources flag, uint32 hash, int line, bool bBreak);
	const ResourceList *GetResourceList() const { return &m_resourceList; }
};

extern CResourceFile *g_pResource;
extern cvar_t *pcv_rch_log;

void ClearStringsCache();
