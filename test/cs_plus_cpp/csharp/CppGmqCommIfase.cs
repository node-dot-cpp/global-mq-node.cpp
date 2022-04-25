using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

public enum ErrorCodeT : Int32
{
    NoError = 0,
    UnspecifiedError = 1,
    NullHandle = 2,
    InvalidHandle = 3
};

class RpgEngine
{

    [DllImport("q-based_infra.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern ErrorCodeT initEngine();
    private static void checkError(ErrorCodeT code)
    {
        if (code != ErrorCodeT.NoError)
        {
            string msg = string.Format("Error code = '{}'", code);
            Debug.Print(msg);
            throw new Exception(msg);
        }
    }

    public static void init()
    {
        var ret = initEngine();
        checkError(ret);
    }
}
class GmqTransport : globalmq.marshalling.ITransport
{

    [DllImport("q-based_infra.dll", CallingConvention = CallingConvention.Cdecl)]
	private static extern ErrorCodeT getThisThreadCommMeans(out UIntPtr handle);

    [DllImport("q-based_infra.dll", CallingConvention = CallingConvention.Cdecl)]
	private static extern ErrorCodeT releaseThisThreadForCommMeans(UIntPtr handle);

    [DllImport("q-based_infra.dll", CallingConvention = CallingConvention.Cdecl)]
	private static extern ErrorCodeT getNextMessageSize(UIntPtr handle, out Int32 requiredBufferSize);

    [DllImport("q-based_infra.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern ErrorCodeT getNextMessage(UIntPtr handle, [In, Out] Byte[] buff, Int32 buffsz, out Int32 bytesCopied);

    [DllImport("q-based_infra.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern ErrorCodeT postMessage(UIntPtr handle, Byte[] buff, Int32 buffSize);

	/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	private UIntPtr handle;

	public GmqTransport(UIntPtr handle_) { handle = handle_; }

    private static void checkError(ErrorCodeT code)
    {
        if (code != ErrorCodeT.NoError)
            throw new Exception(String.Format("Error code = '{0}'", code));
    }

	public static GmqTransport getThreadTransport()
	{
        UIntPtr handle_ = UIntPtr.Zero;
		ErrorCodeT retCode = getThisThreadCommMeans(out handle_);
        // TODO: error checking/handling: throws in case of retCode != 0
        checkError(retCode);
		return new GmqTransport(handle_);
	}

	public virtual void postMessage(globalmq.marshalling.BufferT buff)
	{
        int sz = buff.size();
        globalmq.marshalling.ReadIteratorT it = buff.getReadIterator();
        byte[] arr = it.read(sz);
        ErrorCodeT retCode = postMessage(handle, arr, sz);
        // TODO: error checking/handling: throws in case of retCode != 0
        checkError(retCode);
    }

	public void getNextMessageSize(out Int32 sz)
	{
		ErrorCodeT retCode = getNextMessageSize(handle, out sz);
        // TODO: error checking/handling: throws in case of retCode != 0
        checkError(retCode);
    }

    public void getNextMessage(Byte[] buff, out Int32 bytesCopied)
	{
		ErrorCodeT retCode = getNextMessage(handle, buff, buff.Length, out bytesCopied);
        // TODO: error checking/handling: throws in case of retCode != 0
        checkError(retCode);
    }

    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
        public static void test()
        {
            Debug.Log("START CppGmqCommIfase Test");


            Debug.Log("Calling `getThisThreadCommMeans`...");
            long handle = -1;
            var ret = getThisThreadCommMeans(ref handle);
            if (ret != ErrorCodeT.NoError)
                Debug.LogError("Error: " + ret.ToString());
            Debug.Log("Got handle = " + handle);
            if (handle == -1)
                Debug.LogError("Error handle");


            Debug.Log("Calling `getNextMessageSize`...");
            int requiredBufferSize = -1;
            ret = getNextMessageSize(handle, ref requiredBufferSize);
            if (ret != ErrorCodeT.NoError)
                Debug.LogError("Error: " + ret.ToString());
            Debug.Log("Got requiredBufferSize = " + requiredBufferSize);
            if (requiredBufferSize == -1)
                Debug.LogError("Error requiredBufferSize");


            Debug.Log("Calling `getNextMessage`...");
            sbyte[] buff = new sbyte[requiredBufferSize];
            int bytesCopied = -1;
            ret = getNextMessage(handle, buff, ref bytesCopied);
            if (ret != ErrorCodeT.NoError)
                Debug.LogError("Error: " + ret.ToString());
            Debug.Log("Got bytesCopied = " + bytesCopied);
            if (bytesCopied == -1)
                Debug.LogError("Error bytesCopied");
            var sb = new StringBuilder(1024);
            foreach (var item in buff)
                sb.Append(item.ToString() + ", ");
            Debug.Log("Got buff: [" + sb.ToString() + "]");


            Debug.Log("Calling `postMessage`...");
            for (var i = 0; i < buff.Length; ++i)
                buff[i] = Convert.ToSByte(buff.Length - 1 - i);
            ret = postMessage(handle, buff);
            if (ret != ErrorCodeT.NoError)
                Debug.LogError("Error: " + ret.ToString());


            Debug.Log("Calling `releaseThisThreadForCommMeans`...");
            ret = releaseThisThreadForCommMeans(handle);
            if (ret != ErrorCodeT.NoError)
                Debug.LogError("Error: " + ret.ToString());


            Debug.Log("FINISHED CppGmqCommIfase Test");
        }*/
}
