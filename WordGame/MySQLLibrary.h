#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MySQLLibrary.generated.h"


UCLASS()
class CPPDENEME_API UMySQLLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Belirtilen tablodaki tüm kelimeleri (Content sütunu) okur ve dizi olarak döndürür. */
	UFUNCTION(BlueprintCallable, Category = "My SQLite Library")
	static TArray<FString> GetSubcategories(const FString& LanguagePair_TableName);

	/** Yeni bir tablo oluşturur. (Eğer bu isimde tablo zaten varsa false döner). */
	UFUNCTION(BlueprintCallable, Category = "My SQLite Library")
	static bool CreateTable(const FString& NewTableName);
	
	/** Belirtilen tabloya yeni bir kelime ekler. (Kelime zaten varsa eklemez, false döner). */
	UFUNCTION(BlueprintCallable, Category = "My SQLite Library")
	static bool AddWordToTable(const FString& TableName, const FString& WordToAdd);
	
	/** Belirtilen tabloyu veritabanından tamamen siler. */
	UFUNCTION(BlueprintCallable, Category = "My SQLite Library")
	static bool DeleteTable(const FString& TableToDelete);

	/** Belirtilen tablodan belirli bir kelimeyi bulup siler. */
	UFUNCTION(BlueprintCallable, Category = "My SQLite Library")
	static bool DeleteWordFromTable(const FString& TableName, const FString& WordToDelete);

	/** Tablodaki bir kelimeyi yeni bir kelimeyle değiştirir. (Yeni kelime zaten varsa false döner). */
	UFUNCTION(BlueprintCallable, Category = "My SQLite Library")
	static bool UpdateWordInTable(const FString& TableName, const FString& OldWord, const FString& NewWord);

	/** Bir tablonun ismini değiştirir. (Yeni isimde bir tablo zaten varsa false döner). */
	UFUNCTION(BlueprintCallable, Category = "My SQLite Library")
	static bool RenameTable(const FString& OldTableName, const FString& NewTableName);


	// ==============================================================================
	// --- STRING YARDIMCI FONKSİYONLARI ---
	// ==============================================================================

	/** İki string'i alfabetik olarak karşılaştırır, büyük olanı küçük olanın sonuna ekler. */
	UFUNCTION(BlueprintPure, Category = "My String Utilities")
	static FString CombineStringsAlphabetically(const FString& StringA, const FString& StringB);
	
	/** Bir string'i " = " ayıracına göre ikiye böler. Bulunursa true ve iki parça döner. */
	UFUNCTION(BlueprintCallable, Category = "My String Utilities", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SplitStringByEquals(const FString& InString, FString& OutLeftPart, FString& OutRightPart);

	/** StringA, StringB'den alfabetik olarak küçükse (sözlükte önce geliyorsa) true döner. */
	UFUNCTION(BlueprintPure, Category = "My String Utilities")
	static bool IsStringAlphabeticallySmaller(const FString& StringA, const FString& StringB);

	/** Tüm Türkçe karakterleri (İ, I, Ç, Ş vb.) güvenli bir şekilde küçük harfe çevirir. */
	UFUNCTION(BlueprintPure, Category = "My String Utilities")
	static FString ToLowerTurkish(const FString& InString);

	/** İki string'i Türkçe karakter duyarlı, büyük/küçük harf farketmeksizin karşılaştırır. */
	UFUNCTION(BlueprintPure, Category = "My String Utilities")
	static bool EqualIgnoreCaseTurkish(const FString& StringA, const FString& StringB);


	// ==============================================================================
	// --- ARRAY YARDIMCI FONKSİYONLARI ---
	// ==============================================================================

	/** 0'dan (ArraySize - 1)'e kadar sayılardan oluşan, karıştırılmış bir index dizisi oluşturur. */
	UFUNCTION(BlueprintPure, Category = "My Array Utilities")
	static TArray<int32> CreateShuffledIndexArray(int32 ArraySize);

	/** * 4 şıklı soru için (1 doğru, 3 rastgele yanlış) şık indeksleri üretir.*/
	UFUNCTION(BlueprintPure, Category = "My Array Utilities")
	static TArray<int32> GenerateMultipleChoiceOptions(int32 CorrectIndex, int32 TotalPoolSize);
};
