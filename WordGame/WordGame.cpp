#include "MySQLLibrary.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "SQLiteDatabase.h"
#include "SQLitePreparedStatement.h"
#include "Misc/Char.h"
#include "Math/UnrealMathUtility.h"


TArray<FString> UMySQLLibrary::GetSubcategories(const FString& LanguagePair_TableName)
{
	TArray<FString> Subcategories;
	const FString DatabasePath = FPaths::ProjectContentDir() + TEXT("Databases/dictionary.db");

	FSQLiteDatabase Database;
	if (!Database.Open(*DatabasePath, ESQLiteDatabaseOpenMode::ReadOnly))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open database: %s"), *DatabasePath);
		return Subcategories;
	}

	const FString Query = FString::Printf(TEXT("SELECT Content FROM \"%s\";"), *LanguagePair_TableName);

	FSQLitePreparedStatement Statement;
	if (Statement.Create(Database, *Query))
	{
		while (Statement.Step() == ESQLitePreparedStatementStepResult::Row)
		{
			FString ContentValue;
			Statement.GetColumnValueByName(TEXT("Content"), ContentValue);
			Subcategories.Add(ContentValue);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SELECT statement for query: '%s'"), *Query);
	}

	Statement.Destroy();
	Database.Close();

	return Subcategories;
}

bool UMySQLLibrary::CreateTable(const FString& NewTableName)
{
	const FString DatabasePath = FPaths::ProjectContentDir() + TEXT("Databases/dictionary.db");

	FSQLiteDatabase Database;
	if (!Database.Open(*DatabasePath, ESQLiteDatabaseOpenMode::ReadWrite))
	{
		return false;
	}

	const FString Query = FString::Printf(TEXT("CREATE TABLE \"%s\" (Content TEXT);"), *NewTableName);
	bool bSuccess = Database.Execute(*Query);

	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create table (it might already exist): '%s'"), *NewTableName);
	}
	
	Database.Close();
	return bSuccess;
}

bool UMySQLLibrary::AddWordToTable(const FString& TableName, const FString& WordToAdd)
{
	const FString DatabasePath = FPaths::ProjectContentDir() + TEXT("Databases/dictionary.db");

	FSQLiteDatabase Database;
	if (!Database.Open(*DatabasePath, ESQLiteDatabaseOpenMode::ReadWrite)) return false;

	// 1. KONTROL: Kelime zaten var mı?
	const FString CheckQuery = FString::Printf(TEXT("SELECT 1 FROM \"%s\" WHERE Content = ? LIMIT 1;"), *TableName);
	FSQLitePreparedStatement CheckStatement;
	if (CheckStatement.Create(Database, *CheckQuery))
	{
		CheckStatement.SetBindingValueByIndex(1, WordToAdd);
		if (CheckStatement.Step() == ESQLitePreparedStatementStepResult::Row)
		{
			// Kelime zaten var, işlemi iptal et ve false dön.
			CheckStatement.Destroy();
			Database.Close();
			return false; 
		}
	}
	CheckStatement.Destroy();

	// 2. İŞLEM: Kelime yoksa ekle.
	const FString InsertQuery = FString::Printf(TEXT("INSERT INTO \"%s\" (Content) VALUES (?);"), *TableName);
	FSQLitePreparedStatement InsertStatement;
	if (!InsertStatement.Create(Database, *InsertQuery))
	{
		Database.Close();
		return false;
	}

	InsertStatement.SetBindingValueByIndex(1, WordToAdd);
	bool bSuccess = (InsertStatement.Step() == ESQLitePreparedStatementStepResult::Done);

	InsertStatement.Destroy();
	Database.Close();
	return bSuccess;
}

bool UMySQLLibrary::DeleteTable(const FString& TableToDelete)
{
	const FString DatabasePath = FPaths::ProjectContentDir() + TEXT("Databases/dictionary.db");

	FSQLiteDatabase Database;
	if (!Database.Open(*DatabasePath, ESQLiteDatabaseOpenMode::ReadWrite)) return false;

	const FString Query = FString::Printf(TEXT("DROP TABLE IF EXISTS \"%s\";"), *TableToDelete);
	bool bSuccess = Database.Execute(*Query);

	Database.Close();
	return bSuccess;
}

bool UMySQLLibrary::DeleteWordFromTable(const FString& TableName, const FString& WordToDelete)
{
	const FString DatabasePath = FPaths::ProjectContentDir() + TEXT("Databases/dictionary.db");

	FSQLiteDatabase Database;
	if (!Database.Open(*DatabasePath, ESQLiteDatabaseOpenMode::ReadWrite)) return false;

	const FString Query = FString::Printf(TEXT("DELETE FROM \"%s\" WHERE Content = ?;"), *TableName);
	FSQLitePreparedStatement Statement;
	
	if (!Statement.Create(Database, *Query))
	{
		Database.Close();
		return false;
	}

	Statement.SetBindingValueByIndex(1, WordToDelete);
	bool bSuccess = (Statement.Step() == ESQLitePreparedStatementStepResult::Done);

	Statement.Destroy();
	Database.Close();
	return bSuccess;
}

bool UMySQLLibrary::UpdateWordInTable(const FString& TableName, const FString& OldWord, const FString& NewWord)
{
	const FString DatabasePath = FPaths::ProjectContentDir() + TEXT("Databases/dictionary.db");

	FSQLiteDatabase Database;
	if (!Database.Open(*DatabasePath, ESQLiteDatabaseOpenMode::ReadWrite)) return false;

	// 1. KONTROL: Yeni kelime zaten mevcut mu?
	const FString CheckQuery = FString::Printf(TEXT("SELECT 1 FROM \"%s\" WHERE Content = ? LIMIT 1;"), *TableName);
	FSQLitePreparedStatement CheckStatement;
	if (CheckStatement.Create(Database, *CheckQuery))
	{
		CheckStatement.SetBindingValueByIndex(1, NewWord);
		if (CheckStatement.Step() == ESQLitePreparedStatementStepResult::Row)
		{
			// Yeni kelime zaten var, duplicate önlemek için durdur.
			CheckStatement.Destroy();
			Database.Close();
			return false; 
		}
	}
	CheckStatement.Destroy();

	// 2. İŞLEM: Kelimeyi güncelle.
	const FString UpdateQuery = FString::Printf(TEXT("UPDATE \"%s\" SET Content = ? WHERE Content = ?;"), *TableName);
	FSQLitePreparedStatement UpdateStatement;
	if (!UpdateStatement.Create(Database, *UpdateQuery))
	{
		Database.Close();
		return false;
	}

	UpdateStatement.SetBindingValueByIndex(1, NewWord);
	UpdateStatement.SetBindingValueByIndex(2, OldWord);
	bool bSuccess = (UpdateStatement.Step() == ESQLitePreparedStatementStepResult::Done);

	UpdateStatement.Destroy();
	Database.Close();
	return bSuccess;
}

bool UMySQLLibrary::RenameTable(const FString& OldTableName, const FString& NewTableName)
{
	const FString DatabasePath = FPaths::ProjectContentDir() + TEXT("Databases/dictionary.db");

	FSQLiteDatabase Database;
	if (!Database.Open(*DatabasePath, ESQLiteDatabaseOpenMode::ReadWrite)) return false;

	// 1. KONTROL: Yeni isimde tablo zaten var mı?
	const FString CheckQuery = TEXT("SELECT name FROM sqlite_master WHERE type='table' AND name=?;");
	FSQLitePreparedStatement CheckStatement;
	if (CheckStatement.Create(Database, *CheckQuery))
	{
		CheckStatement.SetBindingValueByIndex(1, NewTableName);
		if (CheckStatement.Step() == ESQLitePreparedStatementStepResult::Row)
		{
			CheckStatement.Destroy();
			Database.Close();
			return false;
		}
	}
	CheckStatement.Destroy();

	// 2. İŞLEM: Adını değiştir.
	const FString RenameQuery = FString::Printf(TEXT("ALTER TABLE \"%s\" RENAME TO \"%s\";"), *OldTableName, *NewTableName);
	bool bSuccess = Database.Execute(*RenameQuery);

	Database.Close();
	return bSuccess;
}

// ==============================================================================
// --- STRING YARDIMCI FONKSİYONLARI ---
// ==============================================================================

FString UMySQLLibrary::CombineStringsAlphabetically(const FString& StringA, const FString& StringB)
{
	if (StringA > StringB) return StringB + StringA;
	else return StringA + StringB;
}

bool UMySQLLibrary::SplitStringByEquals(const FString& InString, FString& OutLeftPart, FString& OutRightPart)
{
	return InString.Split(TEXT(" = "), &OutLeftPart, &OutRightPart);
}

bool UMySQLLibrary::IsStringAlphabeticallySmaller(const FString& StringA, const FString& StringB)
{
	return StringA < StringB; //In blueprints I couldn't use that operator or find a function which does this operation. so I writed this function. 
}

FString UMySQLLibrary::ToLowerTurkish(const FString& InString)
{
	if (InString.IsEmpty()) return InString;

	FString Result = InString;

	Result = Result.Replace(TEXT("İ"), TEXT("i"));
	Result = Result.Replace(TEXT("I"), TEXT("ı"));
	Result = Result.Replace(TEXT("Ç"), TEXT("ç"));
	Result = Result.Replace(TEXT("Ş"), TEXT("ş"));
	Result = Result.Replace(TEXT("Ğ"), TEXT("ğ"));
	Result = Result.Replace(TEXT("Ü"), TEXT("ü"));
	Result = Result.Replace(TEXT("Ö"), TEXT("ö"));

	return Result.ToLower();
}

bool UMySQLLibrary::EqualIgnoreCaseTurkish(const FString& StringA, const FString& StringB)
{
	return ToLowerTurkish(StringA).Equals(ToLowerTurkish(StringB));
}

// ==============================================================================
// --- ARRAY YARDIMCI FONKSİYONLARI ---
// ==============================================================================

TArray<int32> UMySQLLibrary::CreateShuffledIndexArray(int32 ArraySize)
{
	TArray<int32> IndexArray;
	if (ArraySize <= 0) return IndexArray;

	IndexArray.Reserve(ArraySize);
	for (int32 i = 0; i < ArraySize; ++i)
	{
		IndexArray.Add(i);
	}

	for (int32 i = IndexArray.Num() - 1; i > 0; --i)
	{
		const int32 j = FMath::RandRange(0, i);
		IndexArray.Swap(i, j);
	}

	return IndexArray;
}

TArray<int32> UMySQLLibrary::GenerateMultipleChoiceOptions(int32 CorrectIndex, int32 TotalPoolSize)
{
	TArray<int32> Options;

	// Havuz yeterli değilse sadece doğru cevabı dön.
	if (TotalPoolSize < 4)
	{
		Options.Add(CorrectIndex);
		return Options; 
	}

	// 1. Doğru cevabı ekle
	Options.Add(CorrectIndex);

	// 2. 3 adet benzersiz yanlış cevap bul
	while (Options.Num() < 4)
	{
		int32 RandomIndex = FMath::RandRange(0, TotalPoolSize - 1);
		
		if (!Options.Contains(RandomIndex))
		{
			Options.Add(RandomIndex);
		}
	}

	// 3. Şıkları karıştır (Doğru cevap rastgele bir yere gitsin)
	for (int32 i = Options.Num() - 1; i > 0; --i)
	{
		const int32 j = FMath::RandRange(0, i);
		Options.Swap(i, j);
	}

	return Options;
}
