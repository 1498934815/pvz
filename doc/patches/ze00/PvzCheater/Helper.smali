.class public Lze00/PvzCheater/Helper;
.super Ljava/lang/Object;
.source "Helper.java"


# direct methods
.method public constructor <init>()V
    .registers 1

    .prologue
    .line 6
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method

.method public static deflateObb(Landroid/content/Context;Ljava/lang/String;)V
    .registers 10
    .param p0, "ctx"    # Landroid/content/Context;
    .param p1, "obbname"    # Ljava/lang/String;

    .prologue
    .line 8
    const-string/jumbo v6, ".obb"

    invoke-virtual {p1, v6}, Ljava/lang/String;->endsWith(Ljava/lang/String;)Z

    move-result v6

    if-nez v6, :cond_a

    .line 9
    return-void

    .line 10
    :cond_a
    new-instance v3, Ljava/io/File;

    invoke-direct {v3, p1}, Ljava/io/File;-><init>(Ljava/lang/String;)V

    .line 11
    .local v3, "obb":Ljava/io/File;
    invoke-virtual {v3}, Ljava/io/File;->exists()Z

    move-result v6

    if-nez v6, :cond_4a

    .line 13
    :try_start_15
    new-instance v6, Ljava/io/File;

    invoke-virtual {v3}, Ljava/io/File;->getParent()Ljava/lang/String;

    move-result-object v7

    invoke-direct {v6, v7}, Ljava/io/File;-><init>(Ljava/lang/String;)V

    invoke-virtual {v6}, Ljava/io/File;->mkdirs()Z

    .line 14
    invoke-virtual {v3}, Ljava/io/File;->createNewFile()Z

    .line 15
    invoke-virtual {p0}, Landroid/content/Context;->getAssets()Landroid/content/res/AssetManager;

    move-result-object v6

    invoke-virtual {v3}, Ljava/io/File;->getName()Ljava/lang/String;

    move-result-object v7

    invoke-virtual {v6, v7}, Landroid/content/res/AssetManager;->open(Ljava/lang/String;)Ljava/io/InputStream;

    move-result-object v5

    .line 16
    .local v5, "stream":Ljava/io/InputStream;
    new-instance v4, Ljava/io/FileOutputStream;

    invoke-direct {v4, v3}, Ljava/io/FileOutputStream;-><init>(Ljava/io/File;)V

    .line 18
    .local v4, "outs":Ljava/io/FileOutputStream;
    const/high16 v6, 0x100000

    new-array v1, v6, [B

    .line 19
    .local v1, "content":[B
    const/4 v0, 0x0

    .line 20
    .local v0, "cnt":I
    :goto_3a
    invoke-virtual {v5, v1}, Ljava/io/InputStream;->read([B)I

    move-result v0

    const/4 v6, -0x1

    if-eq v0, v6, :cond_4b

    .line 21
    const/4 v6, 0x0

    invoke-virtual {v4, v1, v6, v0}, Ljava/io/FileOutputStream;->write([BII)V

    .line 22
    invoke-virtual {v4}, Ljava/io/FileOutputStream;->flush()V

    goto :goto_3a

    .line 25
    .end local v0    # "cnt":I
    .end local v1    # "content":[B
    .end local v4    # "outs":Ljava/io/FileOutputStream;
    .end local v5    # "stream":Ljava/io/InputStream;
    :catch_49
    move-exception v2

    .line 7
    :cond_4a
    :goto_4a
    return-void

    .line 24
    .restart local v0    # "cnt":I
    .restart local v1    # "content":[B
    .restart local v4    # "outs":Ljava/io/FileOutputStream;
    .restart local v5    # "stream":Ljava/io/InputStream;
    :cond_4b
    invoke-virtual {v4}, Ljava/io/FileOutputStream;->close()V
    :try_end_4e
    .catch Ljava/lang/Exception; {:try_start_15 .. :try_end_4e} :catch_49

    goto :goto_4a
.end method

.method public static native invokePvzServer()V
.end method

.method public static loadPvzServer()V
    .registers 2

    .prologue
    .line 31
    :try_start_0
    const-string/jumbo v1, "pvz_server"

    invoke-static {v1}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    .line 32
    invoke-static {}, Lze00/PvzCheater/Helper;->invokePvzServer()V
    :try_end_9
    .catch Ljava/lang/Throwable; {:try_start_0 .. :try_end_9} :catch_a

    .line 29
    .local v0, "e":Ljava/lang/Throwable;
    :goto_9
    return-void

    .line 33
    .end local v0    # "e":Ljava/lang/Throwable;
    :catch_a
    move-exception v0

    .restart local v0    # "e":Ljava/lang/Throwable;
    goto :goto_9
.end method
